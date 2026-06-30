// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Systems/Investigation/Objectives/BaseVacancyCaseObjective.h"
#include "PlayerObjectiveComponent.generated.h"


class UBaseVacancyCaseObjective;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEAPARTMENT_API UPlayerObjectiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerObjectiveComponent();

	void SetCurrentObjective(UBaseVacancyCaseObjective* NewObjective);
	void ClearActiveObjective();

	void SetActiveObjective(UBaseVacancyCaseObjective* NewObjective);

	UFUNCTION(BlueprintCallable, Category="Objectives")
	bool TryCompleteActiveObjective();

	UFUNCTION(BlueprintCallable, Category="Objectives")
	bool TryFailActiveObjective(const FString& FailReason) const;
	
	UFUNCTION(BlueprintCallable, Category="Objectives")
	UBaseVacancyCaseObjective* GetActiveObjective() const;

	UFUNCTION(BlueprintCallable, Category="Objectives")
	TArray<UBaseVacancyCaseObjective*> GetCompletedObjectives() const;
	
protected:
	
	virtual void BeginPlay() override;

	void InitializePlayerObjectives();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Objectives")
	TArray<TSubclassOf<class UBaseVacancyCaseObjective>> DefaultObjectives;

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Objectives", meta=(AllowPrivateAccess="true"))
	UBaseVacancyCaseObjective* ActiveObjective;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Objectives", meta=(AllowPrivateAccess="true"))
	TArray<UBaseVacancyCaseObjective*> CompletedObjectives;

	bool IsObjectiveComplete(const UBaseVacancyCaseObjective* Objective) const;
};
