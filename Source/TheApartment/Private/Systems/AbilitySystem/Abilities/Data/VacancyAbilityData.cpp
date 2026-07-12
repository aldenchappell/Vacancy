#include "Systems/AbilitySystem/Abilities/Data/VacancyAbilityData.h"

#include "Systems/AbilitySystem/Abilities/VacancyGameplayAbility.h"

bool FVacancyAbilityData::IsValidAbilityData() const
{
	/*
	 * Calling Get() performs TSubclassOf's runtime type validation.
	 * This CPP includes the complete UVacancyGameplayAbility definition.
	 */
	return AbilityClass.Get() != nullptr
		&& AbilityTagData.AbilityTag.IsValid();
}

FGameplayTag FVacancyAbilityData::GetAbilityTag() const
{
	return AbilityTagData.AbilityTag;
}

FGameplayTag FVacancyAbilityData::GetInputTag() const
{
	return AbilityTagData.InputTag;
}