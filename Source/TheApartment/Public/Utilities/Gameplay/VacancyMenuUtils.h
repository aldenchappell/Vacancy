// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/VacancyHUDData.h"
#include "VacancyMenuUtils.generated.h"

class APlayerController;
class AVacancyHUD;
class UUserWidget;

UCLASS()
class THEAPARTMENT_API UVacancyMenuUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Menus", meta = (WorldContext = "WorldContextObject"))
	static APlayerController* GetPlayerControllerFromContext(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Menus", meta = (WorldContext = "WorldContextObject"))
	static AVacancyHUD* GetVacancyHUDFromContext(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Menus", meta = (WorldContext = "WorldContextObject"))
	static bool SetGamePaused(const UObject* WorldContextObject, bool bPaused);

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Menus", meta = (WorldContext = "WorldContextObject"))
	static bool ApplyGameOnlyInputMode(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Menus", meta = (WorldContext = "WorldContextObject"))
	static bool ApplyUIOnlyInputMode(const UObject* WorldContextObject, UUserWidget* WidgetToFocus);

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Menus", meta = (WorldContext = "WorldContextObject"))
	static bool ApplyGameAndUIInputMode(const UObject* WorldContextObject, UUserWidget* WidgetToFocus);

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Menus", meta = (WorldContext = "WorldContextObject"))
	static bool ShowHUDScreen(const UObject* WorldContextObject, EVacancyHUDType HUDType);

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Menus", meta = (WorldContext = "WorldContextObject"))
	static bool SetHUDScreenVisible(const UObject* WorldContextObject, EVacancyHUDType HUDType, bool bVisible);

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Menus", meta = (WorldContext = "WorldContextObject"))
	static bool ResumeGame(const UObject* WorldContextObject, bool bShowInGameHUD = true);

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Menus", meta = (WorldContext = "WorldContextObject"))
	static bool OpenPauseMenu(const UObject* WorldContextObject, UUserWidget* PauseWidgetToFocus);

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Menus", meta = (WorldContext = "WorldContextObject"))
	static bool ReturnToMainMenu(const UObject* WorldContextObject, FName MainMenuLevelName);

	UFUNCTION(BlueprintCallable, Category = "Vacancy|Menus", meta = (WorldContext = "WorldContextObject"))
	static bool QuitGame(const UObject* WorldContextObject);
};