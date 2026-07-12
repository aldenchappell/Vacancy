

#include "Systems/AbilitySystem/Abilities/ToolAbilities/VacancyTakePhotoAbility.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Components/Characters/Player/ProgressionComponents/Camera/PlayerCameraComponent.h"

UVacancyTakePhotoAbility::UVacancyTakePhotoAbility()
{
	InputBehavior = EVacancyAbilityInputBehavior::Instant;
	RequiredToolType = EToolType::Camera;
}

bool UVacancyTakePhotoAbility::
CanActivateVacancyAbility_Implementation(
	AVacancyPlayerCharacter* ActivatingPlayer) const
{
	if (!Super::CanActivateVacancyAbility_Implementation(
		ActivatingPlayer))
	{
		return false;
	}

	const UWorld* World = GetWorld();
	if (!IsValid(World)
		|| World->GetTimeSeconds() < NextAllowedPhotoTime)
	{
		return false;
	}

	const UPlayerCameraComponent* CameraComponent =
		ActivatingPlayer->GetPlayerCameraComponent();

	return IsValid(CameraComponent)
		&& CameraComponent->CanTakePhoto();
}

void UVacancyTakePhotoAbility::
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
		|| !CameraComponent->TryTakePhoto())
	{
		K2_CancelAbility();
		return;
	}

	if (const UWorld* World = GetWorld())
	{
		NextAllowedPhotoTime =
			World->GetTimeSeconds() + PhotoCooldownSeconds;
	}
}