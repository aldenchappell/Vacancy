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

UVacancyInteractionBase* UInteractableUtils::GetInteractionFromIdentifier(const AActor* InActor,
	const FGameplayTag& InteractionIdentifier)
{
	if (!IsValid(InActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetInteractionFromIdentifier called with null InActor."));
		return nullptr;
	}

	if (InteractionIdentifier.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetInteractionFromIdentifier called with empty InteractionIdentifier."));
		return nullptr;
	}

	if (!InActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("GetInteractionFromIdentifier called with an actor that does not implement the InteractableInterface: %s"),
			*InActor->GetName());
		return nullptr;
	}

	const UVacancyInteractionBase* OwnedInteraction = IInteractableInterface::Execute_GetInteraction(InActor);
	if (!IsValid(OwnedInteraction))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetInteractionFromIdentifier: %s implements InteractableInterface but returned null interaction."),
			*InActor->GetName());
		return nullptr;
	}

	if (OwnedInteraction->GetInteractionInfo().InteractionVisualInfo.InteractionTag != InteractionIdentifier)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetInteractionFromIdentifier: Interaction identifier mismatch. Expected: %s, Found: %s"),
			*InteractionIdentifier.ToString(), *OwnedInteraction->GetInteractionInfo().InteractionVisualInfo.InteractionTag.ToString());
		return nullptr;
	}

	return const_cast<UVacancyInteractionBase*>(OwnedInteraction);
}

AActor* UInteractableUtils::GetInteractableActorFromInteraction(const UVacancyInteractionBase* Interaction)
{
	if (!IsValid(Interaction))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetInteractableActorFromInteraction called with null Interaction."));
		return nullptr;
	}

	const AActor* InteractableActor = Interaction->GetInteractionInfo().InteractionBasicInfo.InteractableActor;
	if (!IsValid(InteractableActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetInteractableActorFromInteraction: Interaction has null InteractableActor."));
		return nullptr;
	}

	return const_cast<AActor*>(InteractableActor);
}
