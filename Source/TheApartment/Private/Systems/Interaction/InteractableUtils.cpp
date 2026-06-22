// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Interaction/InteractableUtils.h"

#include "Components/Characters/Player/Interaction/InteractableInterface.h"
#include "Systems/Interaction/Interactions/VacancyInteractionBase.h"

UVacancyInteractionBase* UInteractableUtils::GetInteractionFromActor(const AActor* InActor)
{
	if (IsValid(InActor) && InActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		return IInteractableInterface::Execute_GetInteraction(InActor);
	}
	
	return nullptr;
}

bool UInteractableUtils::ConstructInteractionFromActor(AActor* InActor,
	UVacancyInteractionBase*& OutInteraction)
{
	if (const UVacancyInteractionBase* Interaction = GetInteractionFromActor(InActor))
	{
		OutInteraction = NewObject<UVacancyInteractionBase>(InActor, Interaction->GetClass());
		OutInteraction->InitializeInteraction(Interaction->GetInteractionInfo());
		return true;
	}

	
	return false;
}

FTransform UInteractableUtils::GetInteractionTransformFromActor(const AActor* InActor)
{
	if (IsValid(InActor) && InActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		if (const USceneComponent* TransformComponent = IInteractableInterface::Execute_GetInteractionTransformComponent(InActor))
		{
			return TransformComponent->GetComponentTransform();
		}
	}
	
	return FTransform::Identity;
}
