// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/Objectives/VacancyObjectiveUtils.h"

#include "Systems/Investigation/Objectives/BaseVacancyCaseObjective.h"

UBaseVacancyCaseObjective* UVacancyObjectiveUtils::SpawnObjective(const UObject* WorldContextObject,
	const TSubclassOf<UBaseVacancyCaseObjective> ObjectiveClass)
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
		if (!IsValid(Objective))	
		{
			UE_LOG(LogTemp, Warning, TEXT("GetObjectiveByID: Encountered an invalid objective in the Objectives array."));
			continue;
		}

		if (const FName CurrentObjectiveID =
			Objective->GetObjectives().Num() > 0 ? Objective->GetObjectives()[0].ObjectiveID :
			NAME_None; CurrentObjectiveID.IsNone())
		{
			UE_LOG(LogTemp, Warning, TEXT("GetObjectiveByID: Objective %s has no valid ObjectiveID."), *Objective->GetName());
			continue;
		}
		
		for (const FName& ID: ObjectiveIDs)
		{
			if (ID == ObjectiveID)
			{
				return Objective;
			}
		}
	}

	return nullptr;
}

bool UVacancyObjectiveUtils::IsObjectiveActive(const UBaseVacancyCaseObjective* Objective, const int32 ObjectiveIndex, const FName& ObjectiveID)
{
	if (!IsValid(Objective))
	{
		UE_LOG(LogTemp, Warning, TEXT("IsObjectiveActive called with an invalid Objective."));
		return false;
	}

	if (ObjectiveID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("IsObjectiveActive called with an invalid ObjectiveID."));
		return false;
	}

	const EVacancyCaseObjectiveStatus ObjectiveStatus =
		GetObjectiveState(Objective, ObjectiveIndex, ObjectiveID);

	return ObjectiveStatus == EVacancyCaseObjectiveStatus::InProgress;
}

bool UVacancyObjectiveUtils::IsObjectiveComplete(const UBaseVacancyCaseObjective* Objective, const int32 ObjectiveIndex, const FName& ObjectiveID)
{
	if (!IsValid(Objective))
	{
		UE_LOG(LogTemp, Warning, TEXT("IsObjectiveComplete called with an invalid Objective."));
		return false;
	}

	if (ObjectiveID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("IsObjectiveComplete called with an invalid ObjectiveID."));
		return false;
	}

	const EVacancyCaseObjectiveStatus ObjectiveStatus =
		GetObjectiveState(Objective, ObjectiveIndex, ObjectiveID);

	return ObjectiveStatus == EVacancyCaseObjectiveStatus::Completed;
}

EVacancyCaseObjectiveStatus UVacancyObjectiveUtils::GetObjectiveState(const UBaseVacancyCaseObjective* Objective, const int32 ObjectiveIndex,
	const FName& ObjectiveID)
{
	if (!IsValid(Objective))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetObjectiveState called with an invalid Objective."));
		return EVacancyCaseObjectiveStatus::MAX;
	}

	if (ObjectiveID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetObjectiveState called with an invalid ObjectiveID."));
		return EVacancyCaseObjectiveStatus::MAX;
	}

	return Objective->GetObjectives().Num() > 0 ?
		Objective->GetObjectives()[ObjectiveIndex].ObjectiveStatus :
		EVacancyCaseObjectiveStatus::MAX;
}
