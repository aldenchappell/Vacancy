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
#include "UI/Inventory/Evidence/PlayerCaseInventoryHUD.h"
#include "UI/Inventory/Tools/PlayerToolHUDSuite.h"
#include "UI/Tools/PlayerActiveToolHUD.h"
#include "Utilities/Gameplay/VacancyUIUtils.h"


AVacancyPlayerCharacter::AVacancyPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 0.f; //start in first person.
	SpringArm->bUsePawnControlRotation = false;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

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

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

void AVacancyPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}



void AVacancyPlayerCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVacancyPlayerCharacter::UpdateAnimPropsForEquippedTool(const ABaseTool* EquippedTool) const
{
	if (!IsValid(EquippedTool))
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateAnimPropsForEquippedTool called with null EquippedTool."));
		return;
	}

	const EToolType EquippedToolType = EquippedTool->GetToolData().ToolType;
	UVacancyPlayerAnimInstance* AnimInstance = Cast<UVacancyPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (!IsValid(AnimInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateAnimPropsForEquippedTool failed: AnimInstance is not valid."));
		return;
	}

	AnimInstance->UpdateEquippedToolType(EquippedToolType);
}

void AVacancyPlayerCharacter::UpdateHUDByType(const EVacancyHUDType& HUDType) const
{
	if (HUDType == EVacancyHUDType::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateHUDByType called with HUDType::None. No action taken."));
		return;
	}

	UVacancyUserWidgetBase* HUDWidget = UVacancyUIUtils::GetHUDElementByType(this, HUDType);

	switch (HUDType)
	{
		case EVacancyHUDType::ToolHUD:
			// Logic to update the HUD for ToolHUD
			HUDWidget = UVacancyUIUtils::GetHUDElementByType(this, EVacancyHUDType::ToolHUD);
			UPlayerActiveToolHUD* ToolHUDWidget = Cast<UPlayerActiveToolHUD>(HUDWidget);
			if (!IsValid(ToolHUDWidget))
			{
				UE_LOG(LogTemp, Warning, TEXT("UpdateHUDByType failed: ToolHUDWidget is not valid."));
				return;
			}
			ToolHUDWidget->UpdateToolHUD();
			break;
		case EVacancyHUDType::ToolHUDSuite:
			// Logic to update the HUD for ToolHUDSuite
			HUDWidget = UVacancyUIUtils::GetHUDElementByType(this, EVacancyHUDType::ToolHUDSuite);
			ValidateHUDWidget(HUDWidget);
			UPlayerToolHUDSuite* ToolHUDSuiteWidget = Cast<UPlayerToolHUDSuite>(HUDWidget);
			if (!IsValid(ToolHUDSuiteWidget))
			{
				UE_LOG(LogTemp, Warning, TEXT("UpdateHUDByType failed: ToolHUDSuite widget is not valid."));
				return;
			}
			ToolHUDSuiteWidget->UpdateToolSuiteHUD();
			break;
		case EVacancyHUDType::CaseInventory:
			// Logic to update the HUD for CaseInventory
			HUDWidget = UVacancyUIUtils::GetHUDElementByType(this, EVacancyHUDType::CaseInventory);
			ValidateHUDWidget(HUDWidget);
			UPlayerCaseInventoryHUD* CaseInventoryWidget = Cast<UPlayerCaseInventoryHUD>(HUDWidget);
			if (!IsValid(CaseInventoryWidget))
			{
				UE_LOG(LogTemp, Warning, TEXT("UpdateHUDByType failed: CaseInventory widget is not valid."));
				return;
			}
			CaseInventoryWidget->UpdateCaseInventoryHUD();
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("UpdateHUDByType called with unhandled HUDType: %d"), static_cast<int32>(HUDType));
			break;
	}
}

bool AVacancyPlayerCharacter::ValidateHUDWidget(UVacancyUserWidgetBase*& HUDWidget)
{
	if (!IsValid(HUDWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("ValidateHUDWidget failed: HUDWidget: %s is not valid."), *GetNameSafe(HUDWidget));
		return false;
	}
	return true;
}