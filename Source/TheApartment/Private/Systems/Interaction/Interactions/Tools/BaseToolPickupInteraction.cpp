// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Interaction/Interactions/Tools/BaseToolPickupInteraction.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Components/Characters/Player/ProgressionComponents/BasePlayerProgressionComponent.h"
#include "Components/Characters/Player/Tools/PlayerToolComponent.h"
#include "Systems/Items/Tools/BaseTool.h"
#include "Utilities/Gameplay/VacancyPlayerUtils.h"

bool UBaseToolPickupInteraction::CanInteract_Implementation(AVacancyPlayerCharacter* InteractingCharacter) const
{
	if (!IsValid(InteractingCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("CanInteract called with null InteractingCharacter on %s"), *GetName());
		return false;
	}

	// Default implementation allows interaction. Override in derived classes for specific conditions.
	return Super::CanInteract_Implementation(InteractingCharacter); 
}

void UBaseToolPickupInteraction::Interact_Implementation(AVacancyPlayerCharacter* InteractingCharacter)
{
	if (!IsValid(InteractingCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("Interact called with null InteractingCharacter on %s"), *GetName());
		return; // If the interacting character is null, do nothing.
	}
	
	if (!ToolAttachmentStateInfo.ToolClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ToolToPickupClass is not set for %s. Cannot pick up tool."), *GetName());
		return; // If the tool class is not set, do nothing.
	}

	Super::Interact_Implementation(InteractingCharacter);

	// First we will Unlock the component that is responsible for the tool pickup,
	if (ToolAttachmentStateInfo.ProgressionComponentClass)
	{
		if (UBasePlayerProgressionComponent* ProgressionComponent =
			UVacancyPlayerUtils::GetPlayerProgressionComponentByClass(
				InteractingCharacter,
				ToolAttachmentStateInfo.ProgressionComponentClass))
		{
			ProgressionComponent->UnlockProgressionComponentTool();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Interact called on %s but the player character does not have the required progression component."), *GetName());
			return; // If the player character does not have the required progression component, do nothing.
		}
	}

	//once we unlock the component, we will check to see if auto equip is true on this interaction, if it is we will equip the tool to the player character.
	if (bAutoEquipOnPickup)
	{
		if (UPlayerToolComponent* ToolComponent = InteractingCharacter->GetPlayerToolComponent())
		{
			ToolComponent->UnequipCurrentTool();
			ToolComponent->EquipNewTool(ToolAttachmentStateInfo.AttachedTool);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Interact called on %s but the player character does not have a tool component."), *GetName());
			return; // If the player character does not have a tool component, do nothing.
		}
	}
}
