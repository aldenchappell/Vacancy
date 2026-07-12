#include "Systems/AbilitySystem/Abilities/ToolAbilities/BaseToolAbility.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Components/Characters/Player/Tools/PlayerToolComponent.h"
#include "Systems/Items/Tools/BaseTool.h"


void UBaseToolAbility::OnAbilityActivated_Implementation(
	AVacancyPlayerCharacter* ActivatingPlayer)
{
	Super::OnAbilityActivated_Implementation(ActivatingPlayer);

	ActiveTool = ResolveEquippedTool(ActivatingPlayer);

	if (!ActiveTool.IsValid())
	{
		K2_CancelAbility();
		return;
	}

	OnToolAbilityActivated(
		ActivatingPlayer,
		ActiveTool.Get());
}

void UBaseToolAbility::OnAbilityEnded_Implementation(
	AVacancyPlayerCharacter* EndingPlayer,
	const bool bWasCancelled)
{
	if (ActiveTool.IsValid())
	{
		OnToolAbilityEnded(
			EndingPlayer,
			ActiveTool.Get(),
			bWasCancelled);
	}

	ActiveTool.Reset();

	Super::OnAbilityEnded_Implementation(
		EndingPlayer,
		bWasCancelled);
}

void UBaseToolAbility::OnToolAbilityActivated_Implementation(
	AVacancyPlayerCharacter* ActivatingPlayer,
	ABaseTool* Tool)
{
}

void UBaseToolAbility::OnToolAbilityEnded_Implementation(
	AVacancyPlayerCharacter* EndingPlayer,
	ABaseTool* Tool,
	bool bWasCancelled)
{
}

ABaseTool* UBaseToolAbility::ResolveEquippedTool(
	const AVacancyPlayerCharacter* Player)
{
	if (!IsValid(Player))
	{
		return nullptr;
	}

	const UPlayerToolComponent* ToolComponent =
		Player->GetPlayerToolComponent();

	if (!IsValid(ToolComponent))
	{
		return nullptr;
	}

	return ToolComponent->GetEquippedTool();
}

bool UBaseToolAbility::CanActivateVacancyAbility_Implementation(
	AVacancyPlayerCharacter* ActivatingPlayer) const
{
	if (!Super::CanActivateVacancyAbility_Implementation(
		ActivatingPlayer))
	{
		return false;
	}

	const ABaseTool* EquippedTool =
		ResolveEquippedTool(ActivatingPlayer);

	if (!IsValid(EquippedTool))
	{
		return false;
	}

	return EquippedTool->GetToolData().ToolType
		== RequiredToolType;
}