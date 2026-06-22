// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VacancyPlayerController.generated.h"

class AVacancyHUD;
/**
 * 
 */
UCLASS()
class THEAPARTMENT_API AVacancyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category="HUD")
	AVacancyHUD* GetVacancyHUD() const;
};
