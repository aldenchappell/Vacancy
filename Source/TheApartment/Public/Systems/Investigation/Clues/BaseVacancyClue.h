// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VacancyCaseData.h"
#include "UObject/Object.h"
#include "BaseVacancyClue.generated.h"

/**
 * 
 */
UCLASS()
class THEAPARTMENT_API UBaseVacancyClue : public UObject
{
	GENERATED_BODY()

public:

	void InitializeClue();

	UFUNCTION(BlueprintCallable, Category="Clue")
	FORCEINLINE FName GetClueID() const { return ClueDataHandle.RowName; }

protected:

	UFUNCTION(BlueprintNativeEvent, Category="Clue")
	void OnClueInitialized();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Clue")
	FDataTableRowHandle ClueDataHandle;
};
