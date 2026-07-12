#include "Systems/AbilitySystem/VacancyAbilitySystemComponent.h"

#include "Systems/AbilitySystem/Abilities/VacancyGameplayAbility.h"

UVacancyAbilitySystemComponent::UVacancyAbilitySystemComponent()
{
	/*
	 * Harmless in single-player and allows ability specs/tags to replicate if
	 * multiplayer is added later.
	 */
	SetIsReplicatedByDefault(true);
}

bool UVacancyAbilitySystemComponent::TryGiveAbilityByClass(
	const TSubclassOf<UVacancyGameplayAbility> AbilityClass)
{
	if (!AbilityClass || !IsOwnerActorAuthoritative())
	{
		return false;
	}

	/*
	 * Treat an already-granted ability as success.
	 * This makes acquisition and equipment code safely idempotent.
	 */
	if (FindAbilitySpecFromClass(AbilityClass) != nullptr)
	{
		return true;
	}

	const UVacancyGameplayAbility* AbilityCDO =
		AbilityClass.GetDefaultObject();

	if (!IsValid(AbilityCDO))
	{
		return false;
	}

	const FVacancyAbilityData* AbilityData =
		AbilityCDO->GetVacancyAbilityDataPtr();

	if (AbilityData == nullptr || !AbilityData->IsValidAbilityData())
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Cannot grant ability %s because its ability data is invalid."),
			*GetNameSafe(AbilityClass.Get()));

		return false;
	}

	FGameplayAbilitySpec AbilitySpec(
		AbilityClass,
		1,
		INDEX_NONE,
		nullptr);

	/*
	 * Dynamic spec source tags are replicated with the granted ability spec.
	 *
	 * We use them for:
	 * - The ability's unique identity tag.
	 * - The input tag used to activate the ability.
	 */
	FGameplayTagContainer& DynamicSpecSourceTags =
		AbilitySpec.GetDynamicSpecSourceTags();

	DynamicSpecSourceTags.AddTag(
		AbilityData->AbilityTagData.AbilityTag);

	if (AbilityData->AbilityTagData.InputTag.IsValid())
	{
		DynamicSpecSourceTags.AddTag(
			AbilityData->AbilityTagData.InputTag);
	}

	return GiveAbility(AbilitySpec).IsValid();
}

bool UVacancyAbilitySystemComponent::TryGiveAbilitiesByClass(
	const TArray<TSubclassOf<UVacancyGameplayAbility>>& AbilityClasses)
{
	if (AbilityClasses.IsEmpty())
	{
		return false;
	}

	bool bAllAbilitiesGranted = true;

	for (const TSubclassOf<UVacancyGameplayAbility>& AbilityClass
		: AbilityClasses)
	{
		if (!TryGiveAbilityByClass(AbilityClass))
		{
			bAllAbilitiesGranted = false;
		}
	}

	return bAllAbilitiesGranted;
}

bool UVacancyAbilitySystemComponent::TryRemoveAbilityByClass(
	const TSubclassOf<UVacancyGameplayAbility> AbilityClass)
{
	if (!AbilityClass || !IsOwnerActorAuthoritative())
	{
		return false;
	}

	FGameplayAbilitySpec* AbilitySpec =
		FindAbilitySpecFromClass(AbilityClass);

	if (AbilitySpec == nullptr)
	{
		return false;
	}

	if (AbilitySpec->IsActive())
	{
		/*
		 * Prevents unsafe removal in the middle of activation cleanup.
		 */
		SetRemoveAbilityOnEnd(AbilitySpec->Handle);
	}
	else
	{
		ClearAbility(AbilitySpec->Handle);
	}

	return true;
}

bool UVacancyAbilitySystemComponent::TryRemoveAbilityByTag(
	const FGameplayTag& AbilityTag)
{
	if (!AbilityTag.IsValid() || !IsOwnerActorAuthoritative())
	{
		return false;
	}

	const FGameplayAbilitySpecHandle AbilityHandle =
		FindAbilityHandleByTag(AbilityTag);

	if (!AbilityHandle.IsValid())
	{
		return false;
	}

	FGameplayAbilitySpec* AbilitySpec =
		FindAbilitySpecFromHandle(AbilityHandle);

	if (AbilitySpec == nullptr)
	{
		return false;
	}

	if (AbilitySpec->IsActive())
	{
		SetRemoveAbilityOnEnd(AbilityHandle);
	}
	else
	{
		ClearAbility(AbilityHandle);
	}

	return true;
}

bool UVacancyAbilitySystemComponent::TryActivateVacancyAbility(
	const FVacancyAbilityData& AbilityData)
{
	if (!AbilityData.IsValidAbilityData())
	{
		return false;
	}

	return TryActivateVacancyAbilityByTag(
		AbilityData.GetAbilityTag());
}

bool UVacancyAbilitySystemComponent::TryActivateVacancyAbilityByClass(
	const TSubclassOf<UVacancyGameplayAbility> AbilityClass)
{
	if (!AbilityClass)
	{
		return false;
	}

	return UAbilitySystemComponent::TryActivateAbilityByClass(
		AbilityClass,
		true);
}

bool UVacancyAbilitySystemComponent::TryActivateVacancyAbilityByTag(
	const FGameplayTag& AbilityTag)
{
	if (!AbilityTag.IsValid())
	{
		return false;
	}

	const FGameplayAbilitySpecHandle AbilityHandle =
		FindAbilityHandleByTag(AbilityTag);

	if (!AbilityHandle.IsValid())
	{
		return false;
	}

	return TryActivateAbility(AbilityHandle, true);
}

bool UVacancyAbilitySystemComponent::TryCancelVacancyAbility(
	const FVacancyAbilityData& AbilityData)
{
	if (!AbilityData.IsValidAbilityData())
	{
		return false;
	}

	return TryCancelVacancyAbilityByTag(
		AbilityData.GetAbilityTag());
}

bool UVacancyAbilitySystemComponent::TryCancelVacancyAbilityByClass(
	const TSubclassOf<UVacancyGameplayAbility> AbilityClass)
{
	if (!AbilityClass)
	{
		return false;
	}

	FGameplayAbilitySpec* AbilitySpec =
		FindAbilitySpecFromClass(AbilityClass);

	if (AbilitySpec == nullptr || !AbilitySpec->IsActive())
	{
		return false;
	}

	CancelAbilityHandle(AbilitySpec->Handle);
	return true;
}

bool UVacancyAbilitySystemComponent::TryCancelVacancyAbilityByTag(
	const FGameplayTag& AbilityTag)
{
	if (!AbilityTag.IsValid())
	{
		return false;
	}

	const FGameplayAbilitySpecHandle AbilityHandle =
		FindAbilityHandleByTag(AbilityTag);

	if (!AbilityHandle.IsValid())
	{
		return false;
	}

	FGameplayAbilitySpec* AbilitySpec =
		FindAbilitySpecFromHandle(AbilityHandle);

	if (AbilitySpec == nullptr || !AbilitySpec->IsActive())
	{
		return false;
	}

	CancelAbilityHandle(AbilityHandle);
	return true;
}

bool UVacancyAbilitySystemComponent::TryEndVacancyAbility(
	const FVacancyAbilityData& AbilityData)
{
	/*
	 * External code cannot safely manufacture the full EndAbility context.
	 * Treat an external end request as cancellation.
	 */
	return TryCancelVacancyAbility(AbilityData);
}

void UVacancyAbilitySystemComponent::AbilityInputTagPressed(
	const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	/*
	 * Usually one input activates one ability.
	 * Inline storage prevents a heap allocation for that common case.
	 */
	TArray<FGameplayAbilitySpecHandle, TInlineAllocator<4>>
		AbilitiesToActivate;

	{
		ABILITYLIST_SCOPE_LOCK();

		for (FGameplayAbilitySpec& AbilitySpec
			: GetActivatableAbilities())
		{
			if (!SpecHasExactTag(AbilitySpec, InputTag))
			{
				continue;
			}

			AbilitySpec.InputPressed = true;

			if (AbilitySpec.IsActive())
			{
				/*
				 * Toggle abilities receive their second press here.
				 * Active manual abilities can also react to repeated input.
				 */
				AbilitySpecInputPressed(AbilitySpec);
			}
			else
			{
				AbilitiesToActivate.Add(AbilitySpec.Handle);
			}
		}
	}

	/*
	 * Activate outside the list lock because activation may add/remove or
	 * otherwise modify ability state.
	 */
	for (const FGameplayAbilitySpecHandle& AbilityHandle
		: AbilitiesToActivate)
	{
		TryActivateAbility(AbilityHandle, true);
	}
}

void UVacancyAbilitySystemComponent::AbilityInputTagReleased(
	const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	ABILITYLIST_SCOPE_LOCK();

	for (FGameplayAbilitySpec& AbilitySpec
		: GetActivatableAbilities())
	{
		if (!SpecHasExactTag(AbilitySpec, InputTag))
		{
			continue;
		}

		AbilitySpec.InputPressed = false;

		if (AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

FGameplayAbilitySpecHandle
UVacancyAbilitySystemComponent::FindAbilityHandleByTag(
	const FGameplayTag& AbilityTag)
{
	if (!AbilityTag.IsValid())
	{
		return FGameplayAbilitySpecHandle();
	}

	ABILITYLIST_SCOPE_LOCK();

	for (const FGameplayAbilitySpec& AbilitySpec
		: GetActivatableAbilities())
	{
		if (SpecHasExactTag(AbilitySpec, AbilityTag))
		{
			return AbilitySpec.Handle;
		}
	}

	return FGameplayAbilitySpecHandle();
}

bool UVacancyAbilitySystemComponent::SpecHasExactTag(
	const FGameplayAbilitySpec& AbilitySpec,
	const FGameplayTag& Tag)
{
	if (!Tag.IsValid())
	{
		return false;
	}

	return AbilitySpec
		.GetDynamicSpecSourceTags()
		.HasTagExact(Tag);
}