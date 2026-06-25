// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Characters/Player/Tools/PlayerToolComponent.h"
#include "Systems/Interaction/Interactions/VacancyInteractionBase.h"
#include "BaseToolPickupInteraction.generated.h"

struct FPlayerToolAttachmentStateInfo;
/**
 * 
 */
UCLASS()
class THEAPARTMENT_API UBaseToolPickupInteraction : public UVacancyInteractionBase
{
	GENERATED_BODY()

public:
	
	virtual bool CanInteract_Implementation(AVacancyPlayerCharacter* InteractingCharacter) const override;
	virtual void Interact_Implementation(AVacancyPlayerCharacter* InteractingCharacter) override;

	UPROPERTY(EditAnywhere, Category="Interaction")
	bool bAutoEquipOnPickup = true;

	UPROPERTY(EditAnywhere, Category="Interaction")
	FPlayerToolAttachmentStateInfo ToolAttachmentStateInfo;
};
