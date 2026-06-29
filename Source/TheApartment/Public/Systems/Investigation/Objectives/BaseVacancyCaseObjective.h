// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Investigation/Clues/VacancyCaseData.h"
#include "UObject/Object.h"
#include "BaseVacancyCaseObjective.generated.h"

class AVacancyPlayerCharacter;
class UBaseVacancyClue;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnObjectiveStateChanged,
	EVacancyCaseObjectiveStatus, NewState,
	const AVacancyPlayerCharacter*, PlayerCharacter
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnObjectiveCompleted);

UCLASS()
class THEAPARTMENT_API UBaseVacancyCaseObjective : public UObject
{
	GENERATED_BODY()

public:

	void InitializeObjective();

	bool IsObjectiveActive() const;
	bool IsObjectiveCompleted() const;

	FName GetObjectiveID() const;

	void SetObjectiveStatus(const EVacancyCaseObjectiveStatus NewStatus, const AVacancyPlayerCharacter* PlayerCharacter);
	FORCEINLINE EVacancyCaseObjectiveStatus GetObjectiveStatus() const { return ObjectiveStatus; }
	
	void MarkObjectiveAsActive(const AVacancyPlayerCharacter* PlayerCharacter);
	void MarkObjectiveAsCompleted(const AVacancyPlayerCharacter* PlayerCharacter);
	void MarkObjectiveAsFailed(const AVacancyPlayerCharacter* PlayerCharacter, const FString& FailReason);
	
	UFUNCTION(BlueprintCallable, Category="Case Data")
	bool ShouldDisplayObjective() const;
	
	UFUNCTION(BlueprintCallable, Category="Case Data")
	FORCEINLINE TArray<FVacancyCaseObjectiveStateData> GetObjectivesStateData() const { return ObjectiveData.Objectives; }

	UFUNCTION(BlueprintCallable, Category="Case Data")
	FVacancyCaseObjectiveStateData GetObjectiveAtIndex(const int32 ObjectiveIndex) const;
	
	UFUNCTION(BlueprintCallable, Category="Case Data")
	bool SetObjectiveState(const FName& ObjectiveID, const EVacancyCaseObjectiveStatus NewState);
	
	UPROPERTY(BlueprintAssignable, Category="Case Data")
	FOnObjectiveStateChanged OnObjectiveStateChanged;

	UPROPERTY(BlueprintAssignable, Category="Case Data")
	FOnObjectiveCompleted OnObjectiveCompleted;
	
protected:

	UFUNCTION(BlueprintNativeEvent, Category="Case Data")
	void OnObjectiveInitialized();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Case Data")
	FVacancyCaseObjectiveData ObjectiveData;

	/*
	* Whether the objective has been completed by the player
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Objective Data")
	bool bIsObjectiveCompleted = false;
	
	UFUNCTION(BlueprintNativeEvent, Category="Case Data")
	void HandleEnterActiveState();
	UFUNCTION(BlueprintNativeEvent, Category="Case Data")
	void HandleEnterCompletedState();
	UFUNCTION(BlueprintNativeEvent, Category="Case Data")
	void HandleEnterFailedState();

	
private:

	UFUNCTION()
	void HandleEnterObjectiveState(EVacancyCaseObjectiveStatus NewState, const AVacancyPlayerCharacter* PlayerCharacter);

	static bool DebugObjectiveState();

	/*
	 * Status of the objective, used to determine if the objective is in progress, completed, or failed
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Objective Data", meta=(AllowPrivateAccess="true"))
	EVacancyCaseObjectiveStatus ObjectiveStatus = EVacancyCaseObjectiveStatus::MAX;
	
public:

	UFUNCTION(BlueprintCallable, Category="Case Data")
	FORCEINLINE FVacancyCaseObjectiveData GetObjectiveData() const { return ObjectiveData; }
};


