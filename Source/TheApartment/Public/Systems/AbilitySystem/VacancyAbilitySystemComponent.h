#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Data/VacancyAbilityData.h"
#include "VacancyAbilitySystemComponent.generated.h"

class UVacancyGameplayAbility;

/**
 * Ability System Component used by Vacancy player characters.
 *
 * Input is routed using gameplay tags rather than integer Input IDs.
 */
UCLASS(ClassGroup = (Abilities), meta = (BlueprintSpawnableComponent))
class THEAPARTMENT_API UVacancyAbilitySystemComponent
	: public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	UVacancyAbilitySystemComponent();

	// -------------------------------------------------------------------------
	// Granting
	// -------------------------------------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Abilities")
	bool TryGiveAbilityByClass(
		TSubclassOf<UVacancyGameplayAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Abilities")
	bool TryGiveAbilitiesByClass(
		const TArray<TSubclassOf<UVacancyGameplayAbility>>& AbilityClasses);

	// -------------------------------------------------------------------------
	// Removing
	// -------------------------------------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Abilities")
	bool TryRemoveAbilityByClass(
		TSubclassOf<UVacancyGameplayAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Abilities")
	bool TryRemoveAbilityByTag(const FGameplayTag& AbilityTag);

	// -------------------------------------------------------------------------
	// Activating
	// -------------------------------------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Abilities")
	bool TryActivateVacancyAbility(
		const FVacancyAbilityData& AbilityData);

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Abilities")
	bool TryActivateVacancyAbilityByClass(
		TSubclassOf<UVacancyGameplayAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Abilities")
	bool TryActivateVacancyAbilityByTag(
		const FGameplayTag& AbilityTag);

	// -------------------------------------------------------------------------
	// Cancelling
	// -------------------------------------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Abilities")
	bool TryCancelVacancyAbility(
		const FVacancyAbilityData& AbilityData);

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Abilities")
	bool TryCancelVacancyAbilityByClass(
		TSubclassOf<UVacancyGameplayAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Abilities")
	bool TryCancelVacancyAbilityByTag(
		const FGameplayTag& AbilityTag);

	/**
	 * External systems should cancel active abilities rather than directly
	 * invoking EndAbility.
	 */
	UFUNCTION(
		BlueprintCallable,
		Category = "Vacancy|Abilities",
		meta = (
			DeprecatedFunction,
			DeprecationMessage =
				"External systems should cancel abilities. Abilities end themselves."))
	bool TryEndVacancyAbility(const FVacancyAbilityData& AbilityData);

	// -------------------------------------------------------------------------
	// Input
	// -------------------------------------------------------------------------

	/**
	 * Called when an input represented by InputTag is pressed.
	 */
	void AbilityInputTagPressed(const FGameplayTag& InputTag);

	/**
	 * Called when an input represented by InputTag is released.
	 */
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

private:

	FGameplayAbilitySpecHandle FindAbilityHandleByTag(
		const FGameplayTag& AbilityTag);

	static bool SpecHasExactTag(
		const FGameplayAbilitySpec& AbilitySpec,
		const FGameplayTag& Tag);
};