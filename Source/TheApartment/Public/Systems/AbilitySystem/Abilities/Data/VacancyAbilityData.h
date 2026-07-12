#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Templates/SubclassOf.h"
#include "VacancyAbilityData.generated.h"

class UVacancyGameplayAbility;

/**
 * Describes the broad gameplay purpose of an ability.
 */
UENUM(BlueprintType)
enum class EVacancyAbilityType : uint8
{
	None			UMETA(DisplayName = "None"),
	Tool			UMETA(DisplayName = "Tool"),
	Investigation	UMETA(DisplayName = "Investigation"),
	Utility			UMETA(DisplayName = "Utility")
};

/**
 * Determines how an ability responds to player input.
 */
UENUM(BlueprintType)
enum class EVacancyAbilityInputBehavior : uint8
{
	/**
	 * Activates once and ends immediately.
	 */
	Instant UMETA(DisplayName = "Instant"),

	/**
	 * Remains active until input is released.
	 */
	Hold UMETA(DisplayName = "Hold"),

	/**
	 * First press activates it. Second press ends it.
	 */
	Toggle UMETA(DisplayName = "Toggle"),

	/**
	 * Remains active until explicitly ended.
	 */
	Manual UMETA(DisplayName = "Manual")
};

/**
 * Gameplay tags associated with a Vacancy ability.
 */
USTRUCT(BlueprintType)
struct FVacancyAbilityTagData
{
	GENERATED_BODY()

	/**
	 * Unique identity tag for the ability.
	 *
	 * Example:
	 * Ability.Tool.Flashlight.Toggle
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Data|Tags")
	FGameplayTag AbilityTag;

	/**
	 * Input tag used to activate the ability.
	 *
	 * Example:
	 * Input.Ability.Flashlight
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Data|Tags")
	FGameplayTag InputTag;

	/**
	 * The ability cannot activate while the ASC owns any of these tags.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Data|Tags")
	FGameplayTagContainer ActivationBlockingTags;

	/**
	 * The ASC must own all these tags before activation is permitted.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Data|Tags")
	FGameplayTagContainer RequiredActivationTags;

	/**
	 * Tags added to the ASC while this ability is active.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Data|Tags")
	FGameplayTagContainer GrantedTags;
};

/**
 * Static configuration for a Vacancy gameplay ability.
 */
USTRUCT(BlueprintType)
struct THEAPARTMENT_API FVacancyAbilityData
{
	GENERATED_BODY()

	/**
	 * Ability class represented by this data.
	 *
	 * The full class definition is not needed merely to store the class
	 * reference. Functions that inspect the class are implemented in the CPP.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Data")
	TSubclassOf<UVacancyGameplayAbility> AbilityClass;

	/**
	 * Identity, input, requirement, blocking, and runtime tags.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Data")
	FVacancyAbilityTagData AbilityTagData;

	/**
	 * General category of the ability.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Data")
	EVacancyAbilityType AbilityType = EVacancyAbilityType::None;

	/**
	 * Returns true when this data has a valid Vacancy ability class and
	 * identity tag.
	 *
	 * Implemented in VacancyAbilityData.cpp because inspecting AbilityClass
	 * requires the complete UVacancyGameplayAbility definition.
	 */
	bool IsValidAbilityData() const;

	/**
	 * Returns the unique identity tag for this ability.
	 */
	FGameplayTag GetAbilityTag() const;

	/**
	 * Returns the input tag associated with this ability.
	 */
	FGameplayTag GetInputTag() const;
};