// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/Actor.h"
#include "ObjectiveManager.generated.h"

class UBaseVacancyCaseObjective;
class AVacancyObjectiveActor;

UCLASS()
class THEAPARTMENT_API AObjectiveManager : public AActor
{
	GENERATED_BODY()

public:
	
	AObjectiveManager();

protected:
	
	virtual void BeginPlay() override;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere)
	UBillboardComponent* BillboardComponent;
#endif
	/*
	 * References to all Objective Actors.
	 * Will be used to spawn and manage Objective Actors in the world.
	 */
	UPROPERTY(EditInstanceOnly, Category="Objectives")
	TArray<TObjectPtr<AVacancyObjectiveActor>> ObjectiveActors;

	/*
	 * File path to the objective data asset that contains the objectives for this case.
	 * This will be used to spawn and manage objectives in the world.
	 * Will be used to populate the ObjectiveActors with the correct objectives.
	 * In order for this to work properly, the path must be set to the specific folder where the objective data assets are located.
	 * (Level Specific Paths)
	 */
	UPROPERTY(EditInstanceOnly, Category="Objectives")
	FString LevelSpecificObjectiveDataAssetPath;
	
private:

	void LoadObjectiveClassesAsync();
	void OnObjectiveClassesLoaded(TArray<TSoftClassPtr<UBaseVacancyCaseObjective>> LoadedObjectiveClasses);
	void SpawnObjectivesFromLoadedClasses(const TArray<TSoftClassPtr<UBaseVacancyCaseObjective>>& LoadedObjectiveClasses);
	void AssignObjectivesToObjectiveActors();

	UPROPERTY(VisibleAnywhere, Category="Objectives", meta=(AllowPrivateAccess="true"))
	TArray<TObjectPtr<UBaseVacancyCaseObjective>> SpawnedObjectives;

	TSharedPtr<FStreamableHandle> ObjectiveLoadHandle;
};
