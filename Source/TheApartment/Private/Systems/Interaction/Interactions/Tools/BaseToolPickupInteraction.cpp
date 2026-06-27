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
		return;
	}

	if (!ToolAttachmentStateInfo.ToolClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ToolClass is not set for %s. Cannot pick up tool."), *GetName());
		return;
	}

	if (!ToolAttachmentStateInfo.ProgressionComponentClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProgressionComponentClass is not set for %s. Cannot pick up tool."), *GetName());
		return;
	}

	Super::Interact_Implementation(InteractingCharacter);

	UBasePlayerProgressionComponent* ProgressionComponent =
		UVacancyPlayerUtils::GetPlayerProgressionComponentByClass(
			InteractingCharacter,
			ToolAttachmentStateInfo.ProgressionComponentClass
		);

	if (!IsValid(ProgressionComponent))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("%s could not find progression component class %s on %s."),
			*GetNameSafe(this),
			*GetNameSafe(ToolAttachmentStateInfo.ProgressionComponentClass),
			*GetNameSafe(InteractingCharacter)
		);

		return;
	}

	ProgressionComponent->UnlockProgressionComponentTool();

	if (bAutoEquipOnPickup)
	{
		if (const bool bEquipped = ProgressionComponent->EquipProgressionComponentTool(); !bEquipped)
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("%s unlocked %s but failed to auto-equip its tool."),
				*GetNameSafe(this),
				*GetNameSafe(ProgressionComponent)
			);
		}
	}

	if (OwnerInteractable)
	{
		OwnerInteractable->Destroy();
	}
	else
	{
		if (AActor* OwnerActor = Cast<AActor>(GetOuter()); IsValid(OwnerActor))
		{
			OwnerActor->Destroy();
		}
		else
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("%s has no valid OwnerInteractable or Outer Actor to destroy after interaction."),
				*GetNameSafe(this)
			);
		}
	}
}