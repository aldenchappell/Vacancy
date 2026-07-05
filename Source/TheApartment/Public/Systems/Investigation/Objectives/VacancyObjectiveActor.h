// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseVacancyCaseObjective.h"
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

	void OnObjectiveCompleted(const UBaseVacancyCaseObjective* CompletedObjective, const AVacancyPlayerCharacter* PlayerCharacter);
	
	UFUNCTION(BlueprintCallable, Category="Objective")
	UBaseVacancyCaseObjective* GetOwnerObjective() const { return OwnerObjective; }

	UFUNCTION(BlueprintPure, Category="Objectives")
	FName GetObjectiveID() const { return ObjectiveID; }

	TSubclassOf<UBaseVacancyCaseObjective> GetOwnerObjectiveClass() const;
	
	UFUNCTION(BlueprintCallable, Category="Objective")
	FVector GetObjectiveLocation() const;

	

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Objective")
	FGameplayTag ObjectiveTag;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Objectives")
	FName ObjectiveID = NAME_None;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere)
	UBillboardComponent* ObjectiveIconComponent;
#endif
	
	virtual void Internal_OnObjectiveCompleted(const UBaseVacancyCaseObjective* CompletedObjective, const AVacancyPlayerCharacter* PlayerCharacter);
private:

	void OnOwnerObjectiveSpawned(UBaseVacancyCaseObjective* SpawnedObjective);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Objective", meta=(AllowPrivateAccess="true"))
	UBaseVacancyCaseObjective* OwnerObjective;

	static bool DebugObjectiveActors();
};
