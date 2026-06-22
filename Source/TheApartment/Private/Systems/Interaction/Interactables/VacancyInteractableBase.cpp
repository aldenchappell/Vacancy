// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Interaction/Interactables/VacancyInteractableBase.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Systems/Interaction/InteractableUtils.h"
#include "Systems/Interaction/Interactions/VacancyInteractionBase.h"


AVacancyInteractableBase::AVacancyInteractableBase()
{
	InteractionTransformComponent = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionTransformComponent"));
	InteractionTransformComponent->SetupAttachment(RootComponent);
}

void AVacancyInteractableBase::BeginPlay()
{
	Super::BeginPlay();

	
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
	if (InteractionPtr)
	{
		return InteractionPtr;
	}

	UVacancyInteractionBase* NewInteraction = nullptr;
	AActor* ThisActor = const_cast<AVacancyInteractableBase*>(this); // Cast away constness to allow interaction construction
	UInteractableUtils::ConstructInteractionFromActor(ThisActor, NewInteraction);

	UE_LOG(LogTemp, Warning, TEXT("InteractionPtr is not set for %s. Constructing new interaction and returning it."), *GetName());
	return nullptr;
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
	if (const UVacancyInteractionBase* InteractionInstance = GetInteraction())
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
