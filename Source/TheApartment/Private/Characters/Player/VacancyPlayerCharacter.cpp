// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Player/VacancyPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Characters/Player/VacancyPlayerAnimInstance.h"
#include "Components/Audio/Listener/VacancyAudioListenerComponent.h"
#include "Components/Characters/Player/Evidence/EvidenceInventoryComponent.h"
#include "Components/Characters/Player/Interaction/PlayerInteractionComponent.h"
#include "Components/Characters/Player/PlayerObjectiveComponent/PlayerObjectiveComponent.h"
#include "Components/Characters/Player/ProgressionComponents/Camera/PlayerCameraComponent.h"
#include "Components/Characters/Player/ProgressionComponents/Flashlight/PlayerFlashlightComponent.h"
#include "Components/Characters/Player/ProgressionComponents/Phone/PlayerPhoneComponent.h"
#include "Components/Characters/Player/ProgressionComponents/Recorder/PlayerRecorderComponent.h"
#include "Components/Characters/Player/Suspicion/SuspicionReceiverComponent.h"
#include "Components/Characters/Player/Tools/PlayerToolComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Systems/AbilitySystem/VacancyAbilitySystemComponent.h"
#include "Systems/AbilitySystem/Abilities/VacancyGameplayAbility.h"
#include "Systems/Items/Tools/BaseTool.h"
#include "UI/VacancyHUD.h"
#include "Utilities/Gameplay/VacancyUIUtils.h"

AVacancyPlayerCharacter::AVacancyPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// -------------------------------------------------------------------------
	// Camera Setup
	// -------------------------------------------------------------------------

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 0.f;

	/*
	 * For first-person camera pitch to work through AddPitchInput(),
	 * something in the camera chain needs to use pawn control rotation.
	 *
	 * The spring arm should consume controller rotation.
	 * The camera itself should simply follow the spring arm.
	 */
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	PlayerCamera->bUsePawnControlRotation = false;

	// First-person character rotation setup.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// -------------------------------------------------------------------------
	// Gameplay Components
	// -------------------------------------------------------------------------

	PlayerInteractionComponent = CreateDefaultSubobject<UPlayerInteractionComponent>(TEXT("PlayerInteractionComponent"));
	PlayerObjectiveComponent = CreateDefaultSubobject<UPlayerObjectiveComponent>(TEXT("PlayerObjectiveComponent"));
	AudioListenerComponent = CreateDefaultSubobject<UVacancyAudioListenerComponent>(TEXT("AudioListenerComponent"));
	EvidenceInventoryComponent = CreateDefaultSubobject<UEvidenceInventoryComponent>(TEXT("EvidenceInventoryComponent"));
	SuspicionReceiverComponent = CreateDefaultSubobject<USuspicionReceiverComponent>(TEXT("SuspicionReceiverComponent"));
	PlayerToolComponent = CreateDefaultSubobject<UPlayerToolComponent>(TEXT("PlayerToolComponent"));

	PlayerCameraComponent = CreateDefaultSubobject<UPlayerCameraComponent>(TEXT("PlayerCameraComponent"));
	PlayerFlashlightComponent = CreateDefaultSubobject<UPlayerFlashlightComponent>(TEXT("PlayerFlashlightComponent"));
	PlayerPhoneComponent = CreateDefaultSubobject<UPlayerPhoneComponent>(TEXT("PlayerPhoneComponent"));
	PlayerRecorderComponent = CreateDefaultSubobject<UPlayerRecorderComponent>(TEXT("PlayerRecorderComponent"));

	// -------------------------------------------------------------------------
	// Movement Setup
	// -------------------------------------------------------------------------

	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->NavAgentProps.bCanCrouch = true;
		MovementComponent->bOrientRotationToMovement = false;
	}

	// -------------------------------------------------------------------------
	// Ability System Setup
	// -------------------------------------------------------------------------
	
	AbilitySystemComponent = CreateDefaultSubobject<UVacancyAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void AVacancyPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeAbilitySystem();
	GrantStartupAbilities();
}

void AVacancyPlayerCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVacancyPlayerCharacter::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (!IsValid(EnhancedInputComponent))
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT(
				"%s requires UEnhancedInputComponent, but received %s."),
			*GetNameSafe(this),
			*GetNameSafe(PlayerInputComponent));

		return;
	}

	for (const FVacancyAbilityInputBinding& Binding
		: AbilityInputBindings)
	{
		if (!Binding.IsValid())
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("%s contains an invalid ability input binding."),
				*GetNameSafe(this));

			continue;
		}

		EnhancedInputComponent->BindAction(
			Binding.InputAction,
			ETriggerEvent::Started,
			this,
			&ThisClass::HandleAbilityInputPressed,
			Binding.InputTag);

		EnhancedInputComponent->BindAction(
			Binding.InputAction,
			ETriggerEvent::Completed,
			this,
			&ThisClass::HandleAbilityInputReleased,
			Binding.InputTag);

		/*
		 * Canceled covers cases where an input loses eligibility while held,
		 * such as a mapping context being removed.
		 */
		EnhancedInputComponent->BindAction(
			Binding.InputAction,
			ETriggerEvent::Canceled,
			this,
			&ThisClass::HandleAbilityInputReleased,
			Binding.InputTag);
	}
}

void AVacancyPlayerCharacter::UpdateAnimPropsForEquippedTool(const ABaseTool* EquippedTool) const
{
	if (!IsValid(EquippedTool))
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateAnimPropsForEquippedTool failed: EquippedTool is invalid."));
		return;
	}

	USkeletalMeshComponent* PlayerMesh = GetMesh();
	if (!IsValid(PlayerMesh))
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateAnimPropsForEquippedTool failed: Player mesh is invalid."));
		return;
	}

	UVacancyPlayerAnimInstance* AnimInstance =
		Cast<UVacancyPlayerAnimInstance>(PlayerMesh->GetAnimInstance());

	if (!IsValid(AnimInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateAnimPropsForEquippedTool failed: AnimInstance is invalid."));
		return;
	}

	const EToolType EquippedToolType = EquippedTool->GetToolData().ToolType;
	AnimInstance->UpdateEquippedToolType(EquippedToolType);
}

void AVacancyPlayerCharacter::UpdateHUDByType(const EVacancyHUDElementType& HUDType) const
{
	/*
	 * Backwards-compatible function.
	 *
	 * Old code can keep calling UpdateHUDByType().
	 * New code should call RefreshHUDElement() directly.
	 */
	RefreshHUDElement(HUDType);
}

void AVacancyPlayerCharacter::RefreshHUDElement(const EVacancyHUDElementType HUDElementType) const
{
	if (HUDElementType == EVacancyHUDElementType::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("RefreshHUDElement failed: HUDElementType is None."));
		return;
	}

	AVacancyHUD* VacancyHUD = GetVacancyHUD();
	if (!IsValid(VacancyHUD))
	{
		UE_LOG(LogTemp, Warning, TEXT("RefreshHUDElement failed: VacancyHUD is invalid."));
		return;
	}

	VacancyHUD->RefreshHUDElement(HUDElementType);
}

void AVacancyPlayerCharacter::SetHUDElementVisible(
	const EVacancyHUDElementType HUDElementType,
	const bool bVisible) const
{
	if (HUDElementType == EVacancyHUDElementType::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetHUDElementVisible failed: HUDElementType is None."));
		return;
	}

	AVacancyHUD* VacancyHUD = GetVacancyHUD();
	if (!IsValid(VacancyHUD))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetHUDElementVisible failed: VacancyHUD is invalid."));
		return;
	}

	VacancyHUD->SetHUDElementVisible(HUDElementType, bVisible);
}

void AVacancyPlayerCharacter::RefreshHUDScreen(const EVacancyHUDType HUDType) const
{
	if (HUDType == EVacancyHUDType::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("RefreshHUDScreen failed: HUDType is None."));
		return;
	}

	AVacancyHUD* VacancyHUD = GetVacancyHUD();
	if (!IsValid(VacancyHUD))
	{
		UE_LOG(LogTemp, Warning, TEXT("RefreshHUDScreen failed: VacancyHUD is invalid."));
		return;
	}

	VacancyHUD->RefreshHUDScreen(HUDType);
}

void AVacancyPlayerCharacter::SetHUDScreenVisible(
	const EVacancyHUDType HUDType,
	const bool bVisible) const
{
	if (HUDType == EVacancyHUDType::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetHUDScreenVisible failed: HUDType is None."));
		return;
	}

	AVacancyHUD* VacancyHUD = GetVacancyHUD();
	if (!IsValid(VacancyHUD))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetHUDScreenVisible failed: VacancyHUD is invalid."));
		return;
	}

	VacancyHUD->SetHUDScreenVisible(HUDType, bVisible);
}

void AVacancyPlayerCharacter::ShowOnlyHUDScreen(const EVacancyHUDType HUDType) const
{
	if (HUDType == EVacancyHUDType::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShowOnlyHUDScreen failed: HUDType is None."));
		return;
	}

	AVacancyHUD* VacancyHUD = GetVacancyHUD();
	if (!IsValid(VacancyHUD))
	{
		UE_LOG(LogTemp, Warning, TEXT("ShowOnlyHUDScreen failed: VacancyHUD is invalid."));
		return;
	}

	VacancyHUD->ShowOnlyHUDScreen(HUDType);
}

AVacancyHUD* AVacancyPlayerCharacter::GetVacancyHUD() const
{
	AVacancyHUD* VacancyHUD = UVacancyUIUtils::GetVacancyHUD(this);
	if (!IsValid(VacancyHUD))
	{
		return nullptr;
	}

	return VacancyHUD;
}

#pragma region Ability System

	void AVacancyPlayerCharacter::InitializeAbilitySystem()
{
	if (!IsValid(AbilitySystemComponent))
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("%s does not have a valid AbilitySystemComponent."),
			*GetNameSafe(this));

		return;
	}

	/*
	 * The player owns the ASC and is also its physical avatar.
	 */
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AVacancyPlayerCharacter::GrantStartupAbilities()
{
	if (!HasAuthority() || !IsValid(AbilitySystemComponent))
	{
		return;
	}

	AbilitySystemComponent->TryGiveAbilitiesByClass(
		StartupAbilities);
}

UAbilitySystemComponent*
AVacancyPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

FGameplayTagContainer
AVacancyPlayerCharacter::GetActivePlayerTags() const
{
	FGameplayTagContainer OwnedTags;

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->GetOwnedGameplayTags(OwnedTags);
	}

	return OwnedTags;
}

void AVacancyPlayerCharacter::HandleAbilityInputPressed(
	const FInputActionValue& InputValue,
	const FGameplayTag InputTag)
{
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	AbilitySystemComponent->AbilityInputTagPressed(InputTag);
}

void AVacancyPlayerCharacter::HandleAbilityInputReleased(
	const FInputActionValue& InputValue,
	const FGameplayTag InputTag)
{
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	AbilitySystemComponent->AbilityInputTagReleased(InputTag);
}

#pragma endregion