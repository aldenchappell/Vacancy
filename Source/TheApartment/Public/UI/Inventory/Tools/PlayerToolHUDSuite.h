// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/VacancyUserWidgetBase.h"
#include "PlayerToolHUDSuite.generated.h"

/**
 * 
 */
UCLASS()
class THEAPARTMENT_API UPlayerToolHUDSuite : public UVacancyUserWidgetBase
{
	GENERATED_BODY()

public:

	void UpdateToolSuiteHUD();
};
