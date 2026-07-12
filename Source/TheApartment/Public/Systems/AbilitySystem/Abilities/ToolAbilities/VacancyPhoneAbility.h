// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseToolAbility.h"
#include "VacancyPhoneAbility.generated.h"

/**
 * 
 */
UCLASS()
class THEAPARTMENT_API UVacancyPhoneAbility : public UBaseToolAbility
{
	GENERATED_BODY()

public:

	UVacancyPhoneAbility();

protected:

	virtual bool CanActivateVacancyAbility_Implementation(
		AVacancyPlayerCharacter* ActivatingPlayer) const override;

	virtual void OnToolAbilityActivated_Implementation(
		AVacancyPlayerCharacter* ActivatingPlayer,
		ABaseTool* Tool) override;

	virtual void OnToolAbilityEnded_Implementation(
		AVacancyPlayerCharacter* EndingPlayer,
		ABaseTool* Tool,
		bool bWasCancelled) override;
};
