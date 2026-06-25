// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Interaction/Interactions/VacancyInteractionBase.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Components/Characters/Player/Interaction/InteractionData.h"

static TAutoConsoleVariable<bool> CVarEnableInteractionActorLogging(
	TEXT("Vacancy.Interactables.EnableLogging"),
	false,
	TEXT("Enable logging for interactions."),
	ECVF_Cheat);



void UVacancyInteractionBase::InitializeInteraction(const FInteractionInfo& InteractInfo)
{
	InteractionData = InteractInfo;
	SetInteractionOwner(InteractInfo.InteractionBasicInfo.InteractableActor);
}

void UVacancyInteractionBase::Interact_Implementation(AVacancyPlayerCharacter* InteractingCharacter)
{
	// Default implementation does nothing. Override in derived classes to provide specific interaction behavior.

	if (!InteractingCharacter)
	{
		OnInteractionFailed(InteractingCharacter, TEXT("Interact called with null InteractingCharacter on %s"));
		return; // If the interacting character is null, do nothing.
	}
	
	if (!CanInteract(InteractingCharacter))
	{
		OnInteractionFailed(InteractingCharacter, TEXT("Cannot interact with %s at this time."));
		return; // If interaction is not possible, do nothing.
	}
	
	OnInteractionSuccessful(InteractingCharacter);
}

bool UVacancyInteractionBase::CanInteract_Implementation(AVacancyPlayerCharacter* InteractingCharacter) const
{
	const FGameplayTagContainer PlayerTags = InteractingCharacter->GetActivePlayerTags();
	if (const bool bHasRequiredTags = PlayerTags.HasAll(InteractionData.InteractionValidationData.RequiredInteractionTags); !bHasRequiredTags)
	{
		UE_LOG(LogTemp, Log, TEXT("%s does not have the required tags to interact with %s"), *InteractingCharacter->GetName(), *GetName());
		return false; // If the player does not have the required tags, do nothing.
	}

	return true;
}

void UVacancyInteractionBase::OnInteractionSuccessful(const AVacancyPlayerCharacter* InteractingCharacter) const
{
	if (CVarEnableInteractionActorLogging.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("%s interacted with %s"), *InteractingCharacter->GetName(), *GetName());
	}
}

void UVacancyInteractionBase::OnInteractionFailed(const AVacancyPlayerCharacter* InteractingCharacter, const FString& FailureReason) const
{
	if (CVarEnableInteractionActorLogging.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Log, TEXT("%s failed to interact with %s. Reason: %s"), *InteractingCharacter->GetName(), *GetName(), *FailureReason);
	}
}
