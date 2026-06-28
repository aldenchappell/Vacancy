// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/VacancyUserWidgetBase.h"
#include "PlayerGameplayHUD.generated.h"

class UPlayerObjectiveHUD;
class UPlayerToolHUDSuite;
class UPlayerCaseInventoryHUD;
class UPlayerActiveToolHUD;
/**
 * 
 */
UCLASS()
class THEAPARTMENT_API UPlayerGameplayHUD : public UVacancyUserWidgetBase
{
	GENERATED_BODY()

public:

	

protected:
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/*
	 * HUD Widgets to Initialize
	 * UPlayerActiveToolHUD
	 * UPlayerToolHUDSuite
	 * UPlayerCaseInventoryHUD
	 * UPlayerObjectiveHUD
	 */
	UPROPERTY(EditDefaultsOnly, Category="HUD|Widgets")
	TArray<TSubclassOf<UUserWidget>> HUDWidgetsToInitialize;
	
private:

	//Helper Functions
	void InitializeHUDWidgets();
	
	UPROPERTY()
	TObjectPtr<UPlayerActiveToolHUD> ActiveToolHUDInstance = nullptr;

public:

	UFUNCTION(BlueprintCallable, Category="HUD")
	FORCEINLINE UPlayerActiveToolHUD* GetActiveToolHUD() const { return ActiveToolHUDInstance; }

	UFUNCTION(BlueprintCallable, Category="HUD")
	UVacancyUserWidgetBase* GetHUDWidgetByType(const EVacancyHUDType HUDType) const;
};
