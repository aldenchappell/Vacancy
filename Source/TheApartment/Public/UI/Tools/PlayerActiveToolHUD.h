// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/VacancyUserWidgetBase.h"
#include "PlayerActiveToolHUD.generated.h"

/**
 * 
 */
UCLASS()
class THEAPARTMENT_API UPlayerActiveToolHUD : public UVacancyUserWidgetBase
{
	GENERATED_BODY()

public:

	void UpdateToolHUD();
	
};
