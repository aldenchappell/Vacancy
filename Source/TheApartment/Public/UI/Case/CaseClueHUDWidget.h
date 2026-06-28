// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/VacancyUserWidgetBase.h"
#include "CaseClueHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEAPARTMENT_API UCaseClueHUDWidget : public UVacancyUserWidgetBase
{
	GENERATED_BODY()

public:

protected:

	virtual void OnVacancyWidgetInitialized_Implementation() override;
};
