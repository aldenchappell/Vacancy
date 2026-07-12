

#include "Systems/AbilitySystem/Abilities/ToolAbilities/VacancyPhoneAbility.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Components/Characters/Player/ProgressionComponents/Phone/PlayerPhoneComponent.h"

UVacancyPhoneAbility::UVacancyPhoneAbility()
{
	InputBehavior = EVacancyAbilityInputBehavior::Toggle;
	RequiredToolType = EToolType::Phone;
}

bool UVacancyPhoneAbility::
CanActivateVacancyAbility_Implementation(
	AVacancyPlayerCharacter* ActivatingPlayer) const
{
	if (!Super::CanActivateVacancyAbility_Implementation(
		ActivatingPlayer))
	{
		return false;
	}

	const UPlayerPhoneComponent* PhoneComponent =
		ActivatingPlayer->GetPlayerPhoneComponent();

	return IsValid(PhoneComponent)
		&& PhoneComponent->CanOpenPhone();
}

void UVacancyPhoneAbility::
OnToolAbilityActivated_Implementation(
	AVacancyPlayerCharacter* ActivatingPlayer,
	ABaseTool* Tool)
{
	Super::OnToolAbilityActivated_Implementation(
		ActivatingPlayer,
		Tool);

	UPlayerPhoneComponent* PhoneComponent =
		ActivatingPlayer->GetPlayerPhoneComponent();

	if (!IsValid(PhoneComponent)
		|| !PhoneComponent->SetPhoneOpen(true))
	{
		K2_CancelAbility();
	}
}

void UVacancyPhoneAbility::
OnToolAbilityEnded_Implementation(
	AVacancyPlayerCharacter* EndingPlayer,
	ABaseTool* Tool,
	const bool bWasCancelled)
{
	if (IsValid(EndingPlayer))
	{
		if (UPlayerPhoneComponent* PhoneComponent =
			EndingPlayer->GetPlayerPhoneComponent())
		{
			PhoneComponent->SetPhoneOpen(false);
		}
	}

	Super::OnToolAbilityEnded_Implementation(
		EndingPlayer,
		Tool,
		bWasCancelled);
}