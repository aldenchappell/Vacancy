// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "VacancyObjectiveActor.generated.h"

class UBaseVacancyCaseObjective;

UCLASS()
class THEAPARTMENT_API AVacancyObjectiveActor : public AActor
{
	GENERATED_BODY()

public:

	void SetOwnerObjective(UBaseVacancyCaseObjective* NewOwnerObjective);

	UFUNCTION(BlueprintCallable, Category="Objective")
	UBaseVacancyCaseObjective* GetOwnerObjective() const { return OwnerObjective; }

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Objective")
	FGameplayTag ObjectiveTag;
	
private:

	void OnOwnerObjectiveSpawned(UBaseVacancyCaseObjective* SpawnedObjective);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Objective", meta=(AllowPrivateAccess="true"))
	UBaseVacancyCaseObjective* OwnerObjective;
};
