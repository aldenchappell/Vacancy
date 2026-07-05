// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Investigation/Clues/VacancyCaseData.h"
#include "UObject/Object.h"
#include "BaseVacancyCaseEvidence.generated.h"

/**
 * 
 */
UCLASS()
class THEAPARTMENT_API UBaseVacancyCaseEvidence : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Case Evidence")
	FORCEINLINE FVacancyEvidenceData GetEvidenceData() const { return EvidenceData; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Case Evidence")
	FVacancyEvidenceData EvidenceData;
};
