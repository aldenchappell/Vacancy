// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	FSoftObjectPath ObjectiveDataAssetPath;
	
private:
	
	/*
	 * Spawns all Objectives from ObjectiveActors in the world.
	 */
	void SpawnObjectivesFromActors();

	void PopulateObjectiveActorsObjectives();

	UPROPERTY(VisibleAnywhere, Category="Objectives", meta=(AllowPrivateAccess="true"))
	TArray<TObjectPtr<UBaseVacancyCaseObjective>> SpawnedObjectives;
};
