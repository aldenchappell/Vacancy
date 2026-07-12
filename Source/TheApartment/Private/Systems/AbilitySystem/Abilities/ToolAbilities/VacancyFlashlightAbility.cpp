#include "Systems/AbilitySystem/Abilities/ToolAbilities/VacancyFlashlightAbility.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Components/Characters/Player/ProgressionComponents/Flashlight/PlayerFlashlightComponent.h"

UVacancyFlashlightAbility::UVacancyFlashlightAbility()
{
	InputBehavior = EVacancyAbilityInputBehavior::Toggle;
	RequiredToolType = EToolType::Flashlight;
}

bool UVacancyFlashlightAbility::
CanActivateVacancyAbility_Implementation(
	AVacancyPlayerCharacter* ActivatingPlayer) const
{
	if (!Super::CanActivateVacancyAbility_Implementation(
		ActivatingPlayer))
	{
		return false;
	}

	const UPlayerFlashlightComponent* FlashlightComponent =
		ActivatingPlayer->GetPlayerFlashlightComponent();

	return IsValid(FlashlightComponent)
		&& FlashlightComponent->CanEnableFlashlight();
}

void UVacancyFlashlightAbility::
OnToolAbilityActivated_Implementation(
	AVacancyPlayerCharacter* ActivatingPlayer,
	ABaseTool* Tool)
{
	Super::OnToolAbilityActivated_Implementation(
		ActivatingPlayer,
		Tool);

	UPlayerFlashlightComponent* FlashlightComponent =
		ActivatingPlayer->GetPlayerFlashlightComponent();

	if (!IsValid(FlashlightComponent)
		|| !FlashlightComponent->SetFlashlightEnabled(true))
	{
		K2_CancelAbility();
	}
}

void UVacancyFlashlightAbility::
OnToolAbilityEnded_Implementation(
	AVacancyPlayerCharacter* EndingPlayer,
	ABaseTool* Tool,
	const bool bWasCancelled)
{
	if (IsValid(EndingPlayer))
	{
		if (UPlayerFlashlightComponent* FlashlightComponent =
			EndingPlayer->GetPlayerFlashlightComponent())
		{
			FlashlightComponent->SetFlashlightEnabled(false);
		}
	}

	Super::OnToolAbilityEnded_Implementation(
		EndingPlayer,
		Tool,
		bWasCancelled);
}