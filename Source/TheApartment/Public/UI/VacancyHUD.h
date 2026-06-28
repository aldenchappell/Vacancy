// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "VacancyHUD.generated.h"

class UPlayerToolHUDSuite;
class UPlayerCaseInventoryHUD;
class UPlayerActiveToolHUD;
class UVacancyUserWidgetBase;
/**
 * 
 */
UCLASS()
class THEAPARTMENT_API AVacancyHUD : public AHUD
{
	GENERATED_BODY()

public:
	
	AVacancyHUD();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UPlayerActiveToolHUD* GetActiveToolHUD() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UPlayerCaseInventoryHUD* GetCaseInventoryHUD() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UPlayerToolHUDSuite* GetToolHUDSuite() const;
protected:

	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UVacancyUserWidgetBase> HUDWidgetClass;
};
