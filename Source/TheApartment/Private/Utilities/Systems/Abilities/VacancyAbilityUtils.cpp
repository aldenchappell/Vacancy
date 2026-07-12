#include "Utilities/Systems/Abilities/VacancyAbilityUtils.h"

#include "AbilitySystemInterface.h"
#include "Systems/AbilitySystem/VacancyAbilitySystemComponent.h"

UVacancyAbilitySystemComponent*
UVacancyAbilityUtils::GetAbilitySystemComponentFromActor(
	const AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return nullptr;
	}

	if (const IAbilitySystemInterface* AbilitySystemInterface =
		Cast<IAbilitySystemInterface>(Actor))
	{
		return Cast<UVacancyAbilitySystemComponent>(
			AbilitySystemInterface->GetAbilitySystemComponent());
	}

	/*
	 * Fallback for actors that contain the component but have not yet
	 * implemented IAbilitySystemInterface.
	 */
	return Actor->FindComponentByClass<
		UVacancyAbilitySystemComponent>();
}