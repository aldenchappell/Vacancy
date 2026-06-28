// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseVacancyClueFragment.generated.h"

class UBaseVacancyClue;
/**
 * 
 */
UCLASS()
class THEAPARTMENT_API UBaseVacancyClueFragment : public UObject
{
	GENERATED_BODY()

public:

	void InitializeClueFragment();

protected:

	UFUNCTION(BlueprintNativeEvent, Category="Clue Fragment")
	void OnClueFragmentInitialized();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Clue Fragment")
	TSubclassOf<UBaseVacancyClue> ClueClasses;
};
