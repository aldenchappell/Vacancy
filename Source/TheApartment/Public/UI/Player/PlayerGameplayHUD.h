// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/VacancyUserWidgetBase.h"
#include "PlayerGameplayHUD.generated.h"

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

	UPROPERTY(EditDefaultsOnly, Category="HUD|Widgets")
	TArray<TSubclassOf<UUserWidget>> HUDWidgetsToInitialize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
	TSubclassOf<UPlayerActiveToolHUD> ActiveToolHUDClass;
private:

	//Helper Functions
	void InitializeHUDWidgets();
	
	UPROPERTY()
	TObjectPtr<UPlayerActiveToolHUD> ActiveToolHUDInstance = nullptr;

public:

	UFUNCTION(BlueprintCallable, Category="HUD")
	FORCEINLINE UPlayerActiveToolHUD* GetActiveToolHUD() const { return ActiveToolHUDInstance; }
};
