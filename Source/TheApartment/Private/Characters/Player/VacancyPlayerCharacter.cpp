// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/VacancyPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Characters/Player/VacancyPlayerAnimInstance.h"
#include "Components/Audio/Listener/VacancyAudioListenerComponent.h"
#include "Components/Characters/Player/Evidence/EvidenceInventoryComponent.h"
#include "Components/Characters/Player/Interaction/PlayerInteractionComponent.h"
#include "Components/Characters/Player/ProgressionComponents/Camera/PlayerCameraComponent.h"
#include "Components/Characters/Player/ProgressionComponents/Flashlight/PlayerFlashlightComponent.h"
#include "Components/Characters/Player/ProgressionComponents/Phone/PlayerPhoneComponent.h"
#include "Components/Characters/Player/ProgressionComponents/Recorder/PlayerRecorderComponent.h"
#include "Components/Characters/Player/Suspicion/SuspicionReceiverComponent.h"
#include "Components/Characters/Player/Tools/PlayerToolComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


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

