// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseToolAbility.h"
#include "VacancyTakePhotoAbility.generated.h"

/**
 * 
 */
UCLASS()
class THEAPARTMENT_API UVacancyTakePhotoAbility : public UBaseToolAbility
{
	GENERATED_BODY()

public:

	UVacancyTakePhotoAbility();

protected:

	virtual bool CanActivateVacancyAbility_Implementation(
		AVacancyPlayerCharacter* ActivatingPlayer) const override;

	virtual void OnToolAbilityActivated_Implementation(
		AVacancyPlayerCharacter* ActivatingPlayer,
		ABaseTool* Tool) override;

	/**
	 * Minimum delay between photographs.
	 *
	 * The camera component may perform additional battery and flash checks.
	 */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Vacancy|Abilities|Camera",
		meta = (ClampMin = "0.0"))
	float PhotoCooldownSeconds = 0.5f;

private:

	float NextAllowedPhotoTime = 0.f;
};
