// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/Objectives/VacancyObjectiveUtils.h"

#include "Systems/Investigation/Objectives/BaseVacancyCaseObjective.h"

UBaseVacancyCaseObjective* UVacancyObjectiveUtils::SpawnObjective(UObject* WorldContextObject,
	TSubclassOf<UBaseVacancyCaseObjective> ObjectiveClass)
{
	if (!IsValid(WorldContextObject))
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnObjective called with an invalid WorldContextObject."));
		return nullptr;
	}

	if (!IsValid(ObjectiveClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnObjective called with an invalid ObjectiveClass."));
		return nullptr;
	}

	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnObjective: Unable to get a valid UWorld from the provided WorldContextObject."));
		return nullptr;
	}

	UBaseVacancyCaseObjective* NewObjective = NewObject<UBaseVacancyCaseObjective>(World, ObjectiveClass);
	if (!IsValid(NewObjective))
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnObjective: Failed to create a new instance of the objective class %s."), *ObjectiveClass->GetName());
		return nullptr;
	}

	NewObjective->InitializeObjective();
	return NewObjective;
}

UBaseVacancyCaseObjective* UVacancyObjectiveUtils::GetObjectiveByID(
	const TArray<UBaseVacancyCaseObjective*>& Objectives, const FName& ObjectiveID)
{
	if (ObjectiveID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetObjectiveByID called with an invalid ObjectiveID."));
		return nullptr;
	}

	if (Objectives.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetObjectiveByID called with an empty Objectives array."));
		return nullptr;
	}

	const TArray<FName> ObjectiveIDs = TArray<FName>();
	
	for (UBaseVacancyCaseObjective* Objective: Objectives)
	{
		const FName CurrentObjectiveID = Objective->GetObjectiveID(Objective->GetObjectiveData());
		
		
		for (const FName& ID: ObjectiveIDs)
		{
			if (ID == ObjectiveID)
			{
				return Objective;
			}
		}
	}
}
