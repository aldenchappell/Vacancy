

#include "Systems/AbilitySystem/Abilities/ToolAbilities/VacancyCameraModeAbility.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Components/Characters/Player/ProgressionComponents/Camera/PlayerCameraComponent.h"

UVacancyCameraModeAbility::UVacancyCameraModeAbility()
{
	InputBehavior = EVacancyAbilityInputBehavior::Toggle;
	RequiredToolType = EToolType::Camera;
}

bool UVacancyCameraModeAbility::
CanActivateVacancyAbility_Implementation(
	AVacancyPlayerCharacter* ActivatingPlayer) const
{
	if (!Super::CanActivateVacancyAbility_Implementation(
		ActivatingPlayer))
	{
		return false;
	}

	const UPlayerCameraComponent* CameraComponent =
		ActivatingPlayer->GetPlayerCameraComponent();

	return IsValid(CameraComponent)
		&& CameraComponent->CanEnterCameraMode();
}

void UVacancyCameraModeAbility::
OnToolAbilityActivated_Implementation(
	AVacancyPlayerCharacter* ActivatingPlayer,
	ABaseTool* Tool)
{
	Super::OnToolAbilityActivated_Implementation(
		ActivatingPlayer,
		Tool);

	UPlayerCameraComponent* CameraComponent =
		ActivatingPlayer->GetPlayerCameraComponent();

	if (!IsValid(CameraComponent)
		|| !CameraComponent->SetCameraModeEnabled(true))
	{
		K2_CancelAbility();
	}
}

void UVacancyCameraModeAbility::
OnToolAbilityEnded_Implementation(
	AVacancyPlayerCharacter* EndingPlayer,
	ABaseTool* Tool,
	const bool bWasCancelled)
{
	if (IsValid(EndingPlayer))
	{
		if (UPlayerCameraComponent* CameraComponent =
			EndingPlayer->GetPlayerCameraComponent())
		{
			CameraComponent->SetCameraModeEnabled(false);
		}
	}

	Super::OnToolAbilityEnded_Implementation(
		EndingPlayer,
		Tool,
		bWasCancelled);
}