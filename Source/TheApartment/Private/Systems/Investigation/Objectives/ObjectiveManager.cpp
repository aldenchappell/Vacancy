// Fill out your copyright notice in the Description page of Project Settings.

#include "Systems/Investigation/Objectives/ObjectiveManager.h"

#include "Systems/Investigation/Objectives/BaseVacancyCaseObjective.h"
#include "Systems/Investigation/Objectives/VacancyObjectiveActor.h"
#include "Utilities/Objectives/VacancyObjectiveUtils.h"

AObjectiveManager::AObjectiveManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AObjectiveManager::BeginPlay()
{
	Super::BeginPlay();

	//Spawns objectives from the ObjectiveActors array
	SpawnObjectivesFromActors();

	//NOTE: May come back and make it so we spawn objectives in the level in order of their index in the ObjectiveActors
	//array, but for now we will just spawn them all at once.
}

void AObjectiveManager::SpawnObjectivesFromActors()
{
	if (ObjectiveActors.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ObjectiveActors found in ObjectiveManager."));
		return;
	}

	for (const AVacancyObjectiveActor* ObjectiveActor : ObjectiveActors)
	{
		if (!IsValid(ObjectiveActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid ObjectiveActor found in ObjectiveManager."));
			continue;
		}

		UBaseVacancyCaseObjective* SpawnedObjective =
			UVacancyObjectiveUtils::SpawnObjective(this, ObjectiveActor->GetOwnerObjectiveClass());
		
		if (!IsValid(SpawnedObjective))
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn objective from ObjectiveActor %s."), *ObjectiveActor->GetName());
			continue;
		}

		SpawnedObjectives.Add(SpawnedObjective);
	}
}

void AObjectiveManager::PopulateObjectiveActorsObjectives()
{
	if (ObjectiveActors.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ObjectiveActors found in ObjectiveManager."));
		return;
	}

	const FString FilePath = ObjectiveDataAssetPath.GetAssetPathString();
	if (FilePath.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectiveDataAssetPath is empty in ObjectiveManager."));
		return;
	}

	// First we will load all objective data assets in the specified path.
	const TArray<FAssetData> AssetDataArray = UVacancyObjectiveUtils::LoadObjectiveDataAssets(FilePath);
	if (AssetDataArray.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No objective data assets found in path: %s"), *FilePath);
		return;
	}
	
	// Then, we will iterate through the ObjectiveActors array and assign the corresponding objective data to each actor based on its ObjectiveID.
	for (AVacancyObjectiveActor* ObjectiveActor : ObjectiveActors)
	{
		if (!IsValid(ObjectiveActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid ObjectiveActor found in ObjectiveManager."));
			continue;
		}

		const FName ActorObjectiveID = ObjectiveActor->GetOwnerObjective()->GetObjectiveID();
		UBaseVacancyCaseObjective* CorrespondingObjective = UVacancyObjectiveUtils::GetObjectiveByID(SpawnedObjectives, ActorObjectiveID);

		if (!IsValid(CorrespondingObjective))
		{
			UE_LOG(LogTemp, Warning, TEXT("No corresponding objective found for ObjectiveActor %s with ObjectiveID %s."), *ObjectiveActor->GetName(), *ActorObjectiveID.ToString());
			continue;
		}

		ObjectiveActor->SetOwnerObjective(CorrespondingObjective);
	}
	// Once we iterate through the ObjectiveActors array, we will assign the objectives to their corresponding actors based on their ObjectiveID.
}
