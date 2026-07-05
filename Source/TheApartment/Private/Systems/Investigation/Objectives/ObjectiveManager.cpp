// Fill out your copyright notice in the Description page of Project Settings.

#include "Systems/Investigation/Objectives/ObjectiveManager.h"

#include "Components/BillboardComponent.h"
#include "Engine/AssetManager.h"
#include "Systems/Investigation/Objectives/BaseVacancyCaseObjective.h"
#include "Systems/Investigation/Objectives/VacancyObjectiveActor.h"
#include "Utilities/Objectives/VacancyObjectiveUtils.h"

AObjectiveManager::AObjectiveManager()
{
	PrimaryActorTick.bCanEverTick = false;

	// Set the default path for level-specific objective data assets 
	// this path will be appended with the level name at runtime to load objectives specific to the current level.
	LevelSpecificObjectiveDataAssetPath = TEXT("/Game/Vacancy/Systems/Objectives/Objectives/Level_");

	BillboardComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("ObjectiveManagerBillboard"));
}

void AObjectiveManager::BeginPlay()
{
	Super::BeginPlay();

	LoadObjectiveClassesAsync();
}

void AObjectiveManager::LoadObjectiveClassesAsync()
{
	if (LevelSpecificObjectiveDataAssetPath.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectiveManager failed: LevelSpecificObjectiveDataAssetPath is empty."));
		return;
	}

	const TArray<TSoftClassPtr<UBaseVacancyCaseObjective>> ObjectiveClasses =
		UVacancyObjectiveUtils::FindObjectiveClassesInFolder(LevelSpecificObjectiveDataAssetPath);

	if (ObjectiveClasses.Num() == 0)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("No objective Blueprint classes found in path: %s"),
			*LevelSpecificObjectiveDataAssetPath
		);

		return;
	}

	TArray<FSoftObjectPath> ObjectiveClassPaths;
	for (const TSoftClassPtr<UBaseVacancyCaseObjective>& ObjectiveClass : ObjectiveClasses)
	{
		if (!ObjectiveClass.IsNull())
		{
			ObjectiveClassPaths.Add(ObjectiveClass.ToSoftObjectPath());
		}
	}

	if (ObjectiveClassPaths.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectiveManager failed: no valid objective class paths found."));
		return;
	}

	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();

	ObjectiveLoadHandle = StreamableManager.RequestAsyncLoad(
		ObjectiveClassPaths,
		FStreamableDelegate::CreateUObject(
			this,
			&AObjectiveManager::OnObjectiveClassesLoaded,
			ObjectiveClasses
		)
	);
}

void AObjectiveManager::OnObjectiveClassesLoaded(
	TArray<TSoftClassPtr<UBaseVacancyCaseObjective>> LoadedObjectiveClasses)
{
	SpawnObjectivesFromLoadedClasses(LoadedObjectiveClasses);
	AssignObjectivesToObjectiveActors();
}

void AObjectiveManager::SpawnObjectivesFromLoadedClasses(
	const TArray<TSoftClassPtr<UBaseVacancyCaseObjective>>& LoadedObjectiveClasses)
{
	SpawnedObjectives.Empty();

	for (const TSoftClassPtr<UBaseVacancyCaseObjective>& SoftObjectiveClass : LoadedObjectiveClasses)
	{
		UClass* LoadedClass = SoftObjectiveClass.Get();

		if (!IsValid(LoadedClass))
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("SpawnObjectivesFromLoadedClasses skipped invalid loaded objective class: %s"),
				*SoftObjectiveClass.ToString()
			);

			continue;
		}

		if (!LoadedClass->IsChildOf(UBaseVacancyCaseObjective::StaticClass()))
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("SpawnObjectivesFromLoadedClasses skipped %s because it does not inherit from UBaseVacancyCaseObjective."),
				*GetNameSafe(LoadedClass)
			);

			continue;
		}

		TSubclassOf<UBaseVacancyCaseObjective> ObjectiveClass = LoadedClass;

		UBaseVacancyCaseObjective* SpawnedObjective =
			UVacancyObjectiveUtils::SpawnObjective(this, ObjectiveClass);

		if (!IsValid(SpawnedObjective))
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("Failed to spawn objective from class %s."),
				*GetNameSafe(LoadedClass)
			);

			continue;
		}

		SpawnedObjectives.Add(SpawnedObjective);

		UE_LOG(
			LogTemp,
			Log,
			TEXT("Spawned objective: %s | ID: %s"),
			*GetNameSafe(SpawnedObjective),
			*SpawnedObjective->GetObjectiveID().ToString()
		);
	}
}

void AObjectiveManager::AssignObjectivesToObjectiveActors()
{
	if (ObjectiveActors.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AssignObjectivesToObjectiveActors failed: no ObjectiveActors assigned."));
		return;
	}

	if (SpawnedObjectives.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AssignObjectivesToObjectiveActors failed: no SpawnedObjectives exist."));
		return;
	}

	for (AVacancyObjectiveActor* ObjectiveActor : ObjectiveActors)
	{
		if (!IsValid(ObjectiveActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("AssignObjectivesToObjectiveActors skipped invalid ObjectiveActor."));
			continue;
		}

		const FName ActorObjectiveID = ObjectiveActor->GetObjectiveID();
		if (ActorObjectiveID.IsNone())
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("ObjectiveActor %s has no ObjectiveID set."),
				*GetNameSafe(ObjectiveActor)
			);

			continue;
		}

		UBaseVacancyCaseObjective* CorrespondingObjective =
			UVacancyObjectiveUtils::GetObjectiveByID(SpawnedObjectives, ActorObjectiveID);

		if (!IsValid(CorrespondingObjective))
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("No loaded objective found for ObjectiveActor %s with ObjectiveID %s."),
				*GetNameSafe(ObjectiveActor),
				*ActorObjectiveID.ToString()
			);

			continue;
		}

		ObjectiveActor->SetOwnerObjective(CorrespondingObjective);

		UE_LOG(
			LogTemp,
			Log,
			TEXT("Assigned objective %s with ID %s to actor %s."),
			*GetNameSafe(CorrespondingObjective),
			*ActorObjectiveID.ToString(),
			*GetNameSafe(ObjectiveActor)
		);
	}
}