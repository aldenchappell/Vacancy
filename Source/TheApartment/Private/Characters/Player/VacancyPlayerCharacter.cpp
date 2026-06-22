// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/VacancyPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/Audio/Listener/VacancyAudioListenerComponent.h"
#include "Components/Characters/Player/Evidence/EvidenceInventoryComponent.h"
#include "Components/Characters/Player/Interaction/PlayerInteractionComponent.h"
#include "Components/Characters/Player/Suspicion/SuspicionReceiverComponent.h"
#include "Components/Characters/Player/Tools/PlayerToolComponent.h"
#include "GameFramework/SpringArmComponent.h"


AVacancyPlayerCharacter::AVacancyPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = false;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	PlayerInteractionComponent = CreateDefaultSubobject<UPlayerInteractionComponent>(TEXT("PlayerInteractionComponent"));
	AudioListenerComponent = CreateDefaultSubobject<UVacancyAudioListenerComponent>(TEXT("AudioListenerComponent"));
	EvidenceInventoryComponent = CreateDefaultSubobject<UEvidenceInventoryComponent>(TEXT("EvidenceInventoryComponent"));
	SuspicionReceiverComponent = CreateDefaultSubobject<USuspicionReceiverComponent>(TEXT("SuspicionReceiverComponent"));
	PlayerToolComponent = CreateDefaultSubobject<UPlayerToolComponent>(TEXT("PlayerToolComponent"));
}

void AVacancyPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
void AVacancyPlayerCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

