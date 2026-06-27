// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VacancyUIUtils.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class THEAPARTMENT_API UVacancyUIUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Vacancy UI Utils")
	static void ShowWidget(UUserWidget* Widget, bool bShow);

	UFUNCTION(BlueprintCallable, Category="Vacancy UI Utils")
	static void SetWidgetVisibility(UUserWidget* Widget, ESlateVisibility Visibility);

	UFUNCTION(BlueprintCallable, Category="Vacancy UI Utils")
	static void SetWidgetText(UTextBlock* TextBlock, const FText& NewText);

	UFUNCTION(BlueprintCallable, Category="Vacancy UI Utils")
	static void SetWidgetTextColor(UTextBlock* TextBlock, const FSlateColor& NewColor);
};
