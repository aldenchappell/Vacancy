#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VacancyAbilityUtils.generated.h"

class UVacancyAbilitySystemComponent;

UCLASS()
class THEAPARTMENT_API UVacancyAbilityUtils
	: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Vacancy|Abilities")
	static UVacancyAbilitySystemComponent*
	GetAbilitySystemComponentFromActor(const AActor* Actor);
};