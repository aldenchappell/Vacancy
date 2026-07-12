#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Systems/AbilitySystem/Abilities/Data/VacancyAbilityData.h"
#include "VacancyAbilityDataAsset.generated.h"

/**
 * Stores static configuration for one Vacancy gameplay ability.
 *
 * The ability holds a hard reference to this asset so that activation never
 * performs a synchronous asset load.
 */
UCLASS(BlueprintType)
class THEAPARTMENT_API UVacancyAbilityDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/**
	 * Blueprint-friendly copy of the ability data.
	 */
	UFUNCTION(BlueprintPure, Category = "Vacancy|Abilities")
	FVacancyAbilityData GetAbilityData() const
	{
		return AbilityData;
	}

	/**
	 * C++ accessor that avoids copying the struct.
	 */
	const FVacancyAbilityData& GetAbilityDataRef() const
	{
		return AbilityData;
	}

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vacancy|Abilities")
	FVacancyAbilityData AbilityData;
};