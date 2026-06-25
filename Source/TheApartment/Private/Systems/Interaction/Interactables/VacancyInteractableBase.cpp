// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Interaction/Interactables/VacancyInteractableBase.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Systems/Interaction/InteractableUtils.h"
#include "Systems/Interaction/Interactions/VacancyInteractionBase.h"


AVacancyInteractableBase::AVacancyInteractableBase()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	InteractionCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionCollision"));
	InteractionCollision->SetupAttachment(RootComponent);
	InteractionCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionCollision->SetCollisionObjectType(ECC_WorldDynamic);
	InteractionCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	InteractionTransformComponent = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionTransformComponent"));
	InteractionTransformComponent->SetupAttachment(RootComponent);
}

void AVacancyInteractableBase::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<UVacancyInteractionBase> InteractionClassToUse = nullptr;
	if (IsValid(InteractionClass))
	{
		InteractionClassToUse = InteractionClass;
	}
	else
	{
		InteractionClassToUse = UVacancyInteractionBase::StaticClass();
		UE_LOG(LogTemp, Warning, TEXT("%s has no InteractionClass set. Using default interaction class."), *GetName());
	}

	InteractionPtr = NewObject<UVacancyInteractionBase>(this, InteractionClassToUse);

	if (!IsValid(InteractionPtr))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s failed to create InteractionPtr."), *GetName());
		return;
	}

	FInteractionInfo InteractionInfo = InteractionPtr->GetInteractionInfo();
	InteractionInfo.InteractionBasicInfo.InteractableActor = this;

	InteractionPtr->InitializeInteraction(InteractionInfo);

	UE_LOG(
		LogTemp,
		Log,
		TEXT("%s created interaction instance: %s"),
		*GetName(),
		*GetNameSafe(InteractionPtr)
	);
}


void AVacancyInteractableBase::Interact_Implementation(AVacancyPlayerCharacter* InteractingCharacter)
{
	if (!IsValid(InteractingCharacter))
	{
		return; // If the interacting character is not valid, do nothing.
	}

	if (GetInteraction())
	{
		GetInteraction()->Interact(InteractingCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid interaction found for %s when %s tried to interact."), *GetName(), *InteractingCharacter->GetName());
	}
	
	UE_LOG(LogTemp, Log, TEXT("%s is interacting with %s"), *InteractingCharacter->GetName(), *GetName());
}

void AVacancyInteractableBase::StartInteraction_Implementation(AVacancyPlayerCharacter* InteractingCharacter)
{
	UE_LOG(LogTemp, Log, TEXT("%s started interaction with %s"), *InteractingCharacter->GetName(), *GetName());
}

void AVacancyInteractableBase::EndInteraction_Implementation(AVacancyPlayerCharacter* InteractingCharacter)
{
	UE_LOG(LogTemp, Log, TEXT("%s ended interaction with %s"), *InteractingCharacter->GetName(), *GetName());
}

UVacancyInteractionBase* AVacancyInteractableBase::GetInteraction_Implementation() const
{
	return InteractionPtr;
}

TSubclassOf<UVacancyInteractionBase> AVacancyInteractableBase::GetInteractionClass_Implementation() const
{
	if (IsValid(InteractionClass))
	{
		return InteractionClass;
	}

	UE_LOG(LogTemp, Warning, TEXT("InteractionClass is not set for %s. Returning default interaction class."), *GetName());
	return UVacancyInteractionBase::StaticClass();
}

FInteractionVisualInfo AVacancyInteractableBase::GetInteractionVisualInfo_Implementation() const
{
	if (const UVacancyInteractionBase* InteractionInstance = Execute_GetInteraction(this))
	{
		return InteractionInstance->GetVisualInfo();
	}

	UE_LOG(LogTemp, Warning, TEXT("Interaction instance is not valid for %s. Returning default interaction visual info."), *GetName());
	return FInteractionVisualInfo();
}

USceneComponent* AVacancyInteractableBase::GetInteractionTransformComponent_Implementation() const
{
	return InteractionTransformComponent;
}
