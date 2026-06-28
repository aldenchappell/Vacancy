// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Investigation/Clues/VacancyCaseData.h"
#include "UObject/Object.h"
#include "BaseVacancyCaseObjective.generated.h"

class AVacancyPlayerCharacter;
class UBaseVacancyClue;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnObjectiveStateChanged,
	EVacancyCaseObjectiveStatus, NewState,
	const FName&, ObjectiveID,
	const AVacancyPlayerCharacter*, PlayerCharacter
);


UCLASS()
class THEAPARTMENT_API UBaseVacancyCaseObjective : public UObject
{
	GENERATED_BODY()

public:

	void InitializeObjective();

	bool IsObjectiveActive(const int32 ObjectiveIndex) const;

	static FName GetObjectiveID(const UBaseVacancyCaseObjective*& Objective, const int32 ObjectiveIndex);

	UFUNCTION(BlueprintCallable, Category="Case Data")
	FORCEINLINE TArray<FVacancyCaseObjectiveStateData> GetObjectives() const { return ObjectiveData.Objectives; }

	UFUNCTION(BlueprintCallable, Category="Case Data")
	bool SetObjectiveState(const FName& ObjectiveID, const EVacancyCaseObjectiveStatus NewState);
	
	UPROPERTY(BlueprintAssignable, Category="Case Data")
	FOnObjectiveStateChanged OnObjectiveStateChanged;

	
protected:

	UFUNCTION(BlueprintNativeEvent, Category="Case Data")
	void OnObjectiveInitialized();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Case Data")
	FVacancyCaseObjectiveData ObjectiveData;

	UFUNCTION(BlueprintNativeEvent, Category="Case Data")
	void HandleEnterActiveState();
	UFUNCTION(BlueprintNativeEvent, Category="Case Data")
	void HandleEnterCompletedState();
	UFUNCTION(BlueprintNativeEvent, Category="Case Data")
	void HandleEnterFailedState();
	
private:

	UFUNCTION()
	void HandleEnterObjectiveState(EVacancyCaseObjectiveStatus NewState, const FName& ObjectiveID, const AVacancyPlayerCharacter* PlayerCharacter);

	static bool DebugObjectiveState();
public:

	UFUNCTION(BlueprintCallable, Category="Case Data")
	FORCEINLINE FVacancyCaseObjectiveData GetObjectiveData() const { return ObjectiveData; }
};


