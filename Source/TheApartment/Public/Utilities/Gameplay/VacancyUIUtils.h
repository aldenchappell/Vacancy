// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/VacancyHUDData.h"
#include "VacancyUIUtils.generated.h"

class AVacancyHUD;
class AVacancyPlayerCharacter;
class UVacancyUserWidgetBase;
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
	static AVacancyHUD* GetVacancyHUD(const AVacancyPlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintCallable, Category="Vacancy UI Utils")
	static void ShowWidget(UUserWidget* Widget, bool bShow);

	UFUNCTION(BlueprintCallable, Category="Vacancy UI Utils")
	static void SetWidgetVisibility(UUserWidget* Widget, ESlateVisibility Visibility);

	UFUNCTION(BlueprintCallable, Category="Vacancy UI Utils")
	static void SetWidgetText(UTextBlock* TextBlock, const FText& NewText);

	UFUNCTION(BlueprintCallable, Category="Vacancy UI Utils")
	static void SetWidgetTextColor(UTextBlock* TextBlock, const FSlateColor& NewColor);

	UFUNCTION(BlueprintCallable, Category="Vacancy UI Utils")
	static void FadeInWidget(UUserWidget* Widget, float Duration);

	UFUNCTION(BlueprintCallable, Category="Vacancy UI Utils")
	static void FadeOutWidget(UUserWidget* Widget, float Duration);

	UFUNCTION(BlueprintCallable, Category="Vacancy UI Utils")
	static UVacancyUserWidgetBase* GetHUDElementByType(
		const AVacancyPlayerCharacter* PlayerCharacter,
		const EVacancyHUDType HUDType);
};
