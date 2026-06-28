// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VacancyHUDData.h"
#include "Blueprint/UserWidget.h"
#include "VacancyUserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class THEAPARTMENT_API UVacancyUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void InitializeVacancyWidget();

	UFUNCTION(BlueprintCallable, Category = "Vacancy User Widget Base")
	FVacancyWidgetData GetWidgetData() const { return WidgetData; }

protected:

	UFUNCTION(BlueprintNativeEvent, Category = "Vacancy User Widget Base")
	void OnVacancyWidgetInitialized();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vacancy User Widget Base")
	FVacancyWidgetData WidgetData;
};
