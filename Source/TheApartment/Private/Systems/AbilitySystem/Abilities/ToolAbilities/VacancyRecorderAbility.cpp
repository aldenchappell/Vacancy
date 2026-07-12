


#include "Systems/AbilitySystem/Abilities/ToolAbilities/VacancyRecorderAbility.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Components/Characters/Player/ProgressionComponents/Recorder/PlayerRecorderComponent.h"

UVacancyRecorderAbility::UVacancyRecorderAbility()
{
	InputBehavior = EVacancyAbilityInputBehavior::Toggle;
	RequiredToolType = EToolType::Recorder;
}

bool UVacancyRecorderAbility::
CanActivateVacancyAbility_Implementation(
	AVacancyPlayerCharacter* ActivatingPlayer) const
{
	if (!Super::CanActivateVacancyAbility_Implementation(
		ActivatingPlayer))
	{
		return false;
	}

	const UPlayerRecorderComponent* RecorderComponent =
		ActivatingPlayer->GetPlayerRecorderComponent();

	return IsValid(RecorderComponent)
		&& RecorderComponent->CanStartRecording();
}

void UVacancyRecorderAbility::
OnToolAbilityActivated_Implementation(
	AVacancyPlayerCharacter* ActivatingPlayer,
	ABaseTool* Tool)
{
	Super::OnToolAbilityActivated_Implementation(
		ActivatingPlayer,
		Tool);

	UPlayerRecorderComponent* RecorderComponent =
		ActivatingPlayer->GetPlayerRecorderComponent();

	if (!IsValid(RecorderComponent)
		|| !RecorderComponent->SetRecordingEnabled(true))
	{
		K2_CancelAbility();
	}
}

void UVacancyRecorderAbility::
OnToolAbilityEnded_Implementation(
	AVacancyPlayerCharacter* EndingPlayer,
	ABaseTool* Tool,
	const bool bWasCancelled)
{
	if (IsValid(EndingPlayer))
	{
		if (UPlayerRecorderComponent* RecorderComponent =
			EndingPlayer->GetPlayerRecorderComponent())
		{
			RecorderComponent->SetRecordingEnabled(false);
		}
	}

	Super::OnToolAbilityEnded_Implementation(
		EndingPlayer,
		Tool,
		bWasCancelled);
}