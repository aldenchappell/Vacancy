// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerObjectiveComponent.generated.h"


class UBaseVacancyCaseObjective;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEAPARTMENT_API UPlayerObjectiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerObjectiveComponent();

	bool AddActiveObjective(UBaseVacancyCaseObjective* NewObjective);
	bool RemoveActiveObjective(UBaseVacancyCaseObjective* ObjectiveToRemove);

	UFUNCTION(BlueprintCallable, Category="Objectives")
	bool TryBeginObjectiveByID(const FName& ObjectiveID) const;

	UFUNCTION(BlueprintCallable, Category="Objectives")
	bool TryCompleteObjectiveByID(const FName& ObjectiveID) const;

	UFUNCTION(BlueprintCallable, Category="Objectives")
	bool TryFailObjectiveByID(const FName& ObjectiveID, const FString& FailReason) const;
	
	UFUNCTION(BlueprintCallable, Category="Objectives")
	TArray<UBaseVacancyCaseObjective*> GetActiveObjectives() const;

	UFUNCTION(BlueprintCallable, Category="Objectives")
	TArray<UBaseVacancyCaseObjective*> GetCompletedObjectives() const;
protected:
	virtual void BeginPlay() override;

	void InitializePlayerObjectives();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Objectives")
	TArray<TSubclassOf<class UBaseVacancyCaseObjective>> DefaultObjectives;

	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Objectives", meta=(AllowPrivateAccess="true"))
	TArray<UBaseVacancyCaseObjective*> ActiveObjectives;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Objectives", meta=(AllowPrivateAccess="true"))
	TArray<UBaseVacancyCaseObjective*> CompletedObjectives;

	bool IsObjectiveComplete() const;
};
