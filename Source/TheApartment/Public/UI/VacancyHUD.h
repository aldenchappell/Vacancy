// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "VacancyHUD.generated.h"

/**
 * 
 */
UCLASS()
class THEAPARTMENT_API AVacancyHUD : public AHUD
{
	GENERATED_BODY()

public:
	
	AVacancyHUD();

protected:

	virtual void BeginPlay() override;
	virtual void DrawHUD() override;
	
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<class UInteractionWidget> InteractionWidgetClass;

	UPROPERTY()
	class UInteractionWidget* InteractionWidgetInstance = nullptr;

public:

	UFUNCTION()
	FORCEINLINE UInteractionWidget* GetInteractionWidget() const { return InteractionWidgetInstance; }
};
