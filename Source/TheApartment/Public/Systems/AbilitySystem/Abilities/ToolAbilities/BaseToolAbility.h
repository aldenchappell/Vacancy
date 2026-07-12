#pragma once

#include "CoreMinimal.h"
#include "Systems/AbilitySystem/Abilities/VacancyGameplayAbility.h"
#include "Systems/Items/Tools/ToolData.h"
#include "BaseToolAbility.generated.h"

class ABaseTool;
class AVacancyPlayerCharacter;

UCLASS(Abstract, Blueprintable)
class THEAPARTMENT_API UBaseToolAbility
	: public UVacancyGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Vacancy|Abilities|Tool")
	ABaseTool* GetActiveTool() const
	{
		return ActiveTool.Get();
	}

protected:

	virtual bool CanActivateVacancyAbility_Implementation(
		AVacancyPlayerCharacter* ActivatingPlayer) const override;

	virtual void OnAbilityActivated_Implementation(
		AVacancyPlayerCharacter* ActivatingPlayer) override;

	virtual void OnAbilityEnded_Implementation(
		AVacancyPlayerCharacter* EndingPlayer,
		bool bWasCancelled) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Vacancy|Abilities|Tool")
	void OnToolAbilityActivated(
		AVacancyPlayerCharacter* ActivatingPlayer,
		ABaseTool* Tool);

	virtual void OnToolAbilityActivated_Implementation(
		AVacancyPlayerCharacter* ActivatingPlayer,
		ABaseTool* Tool);

	UFUNCTION(BlueprintNativeEvent, Category = "Vacancy|Abilities|Tool")
	void OnToolAbilityEnded(
		AVacancyPlayerCharacter* EndingPlayer,
		ABaseTool* Tool,
		bool bWasCancelled);

	virtual void OnToolAbilityEnded_Implementation(
		AVacancyPlayerCharacter* EndingPlayer,
		ABaseTool* Tool,
		bool bWasCancelled);

	/**
	 * Tool type that must currently be equipped.
	 */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Vacancy|Abilities|Tool")
	EToolType RequiredToolType = EToolType::MAX;

private:
	static ABaseTool* ResolveEquippedTool(
		const AVacancyPlayerCharacter* Player);

	UPROPERTY(Transient)
	TWeakObjectPtr<ABaseTool> ActiveTool;
};