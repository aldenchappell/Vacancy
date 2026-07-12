#pragma once

#include "CoreMinimal.h"
#include "Systems/AbilitySystem/Abilities/ToolAbilities/BaseToolAbility.h"
#include "VacancyFlashlightAbility.generated.h"

class AVacancyPlayerCharacter;
class ABaseTool;

/**
 * Toggle ability that controls the player's flashlight.
 *
 * Active ability = flashlight on.
 * Ended ability = flashlight off.
 */
UCLASS()
class THEAPARTMENT_API UVacancyFlashlightAbility : public UBaseToolAbility
{
	GENERATED_BODY()

public:

	UVacancyFlashlightAbility();

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