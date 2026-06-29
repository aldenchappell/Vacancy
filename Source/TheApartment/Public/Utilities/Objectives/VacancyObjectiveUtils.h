// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Systems/Investigation/Clues/VacancyCaseData.h"
#include "VacancyObjectiveUtils.generated.h"

class AVacancyPlayerCharacter;
class UBaseVacancyCaseObjective;
/**
 * 
 */
UCLASS()
class THEAPARTMENT_API UVacancyObjectiveUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static UBaseVacancyCaseObjective* SpawnObjective(const UObject* WorldContextObject, TSubclassOf<UBaseVacancyCaseObjective> ObjectiveClass);
	static UBaseVacancyCaseObjective* GetObjectiveByID(const TArray<UBaseVacancyCaseObjective*>& Objectives, const FName& ObjectiveID);
	static FName GetObjectiveID(const UBaseVacancyCaseObjective* Objective, const int32 ObjectiveIndex);
	static bool TryActivateObjectiveByID(UBaseVacancyCaseObjective* Objective, const int32 ObjectiveIndex, const FName& ObjectiveID, const AVacancyPlayerCharacter* PlayerCharacter);
	static bool IsObjectiveActive(const UBaseVacancyCaseObjective* Objective, const int32 ObjectiveIndex, const FName& ObjectiveID);
	static bool IsObjectiveComplete(const UBaseVacancyCaseObjective* Objective, const int32 ObjectiveIndex, const FName& ObjectiveID);
	static EVacancyCaseObjectiveStatus GetObjectiveState(const UBaseVacancyCaseObjective* Objective, const int32 ObjectiveIndex, const FName& ObjectiveID);
};
