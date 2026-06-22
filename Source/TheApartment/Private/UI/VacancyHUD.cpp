// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/VacancyHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Interaction/InteractionWidget.h"

AVacancyHUD::AVacancyHUD()
{
	
}

void AVacancyHUD::BeginPlay()
{
	Super::BeginPlay();

	if (InteractionWidgetClass)
	{
		InteractionWidgetInstance = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		if (InteractionWidgetInstance)
		{
			InteractionWidgetInstance->AddToViewport();
			InteractionWidgetInstance->SwitchInteractionVisibility(false); // Start with the widget hidden
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create InteractionWidget instance in VacancyHUD."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractionWidgetClass is not set in VacancyHUD."));
	}
}

void AVacancyHUD::DrawHUD()
{
	Super::DrawHUD();

	
}
