// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VacancyObjectiveUtils.generated.h"

class UBaseVacancyCaseObjective;
/**
 * 
 */
UCLASS()
class THEAPARTMENT_API UVacancyObjectiveUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static UBaseVacancyCaseObjective* SpawnObjective(UObject* WorldContextObject, TSubclassOf<UBaseVacancyCaseObjective> ObjectiveClass);
	static UBaseVacancyCaseObjective* GetObjectiveByID(const TArray<UBaseVacancyCaseObjective*>& Objectives, const FName& ObjectiveID);
};
