// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VacancyCaseData.h"
#include "Engine/DataAsset.h"
#include "VacancyCaseDataAsset.generated.h"

class UBaseVacancyClue;
/**
 * 
 */
UCLASS()
class THEAPARTMENT_API UVacancyCaseDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Case Data")
	FVacancyCaseFileData CaseFileData;

	UFUNCTION(BlueprintCallable, Category="Case Data")
	FORCEINLINE FVacancyCaseFileData GetCaseFileData() const { return CaseFileData; }
};
