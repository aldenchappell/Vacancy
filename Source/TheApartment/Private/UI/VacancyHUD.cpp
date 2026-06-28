// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/VacancyHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Interaction/InteractionWidget.h"
#include "UI/Inventory/Evidence/PlayerCaseInventoryHUD.h"
#include "UI/Inventory/Tools/PlayerToolHUDSuite.h"
#include "UI/Player/PlayerGameplayHUD.h"
#include "UI/Tools/PlayerActiveToolHUD.h"

AVacancyHUD::AVacancyHUD()
{
}

void AVacancyHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(HUDWidgetClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("HUDWidgetClass is not set in AVacancyHUD."));
		return;
	}

	HUDWidgetInstance = CreateWidget<UPlayerGameplayHUD>(GetWorld(), HUDWidgetClass);
	if (!IsValid(HUDWidgetInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create HUDWidgetInstance in AVacancyHUD."));
		return;
	}
}

void AVacancyHUD::DrawHUD()
{
	Super::DrawHUD();

	
}

UPlayerActiveToolHUD* AVacancyHUD::GetActiveToolHUD() const
{
	if (!IsValid(HUDWidgetInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("HUDWidgetInstance is not valid in AVacancyHUD."));
		return nullptr;
	}

	return Cast<UPlayerActiveToolHUD>(HUDWidgetInstance->GetHUDWidgetByType(EVacancyHUDType::ToolHUD));
}

UPlayerCaseInventoryHUD* AVacancyHUD::GetCaseInventoryHUD() const
{
	if (!IsValid(HUDWidgetInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("HUDWidgetInstance is not valid in AVacancyHUD."));
		return nullptr;
	}

	return Cast<UPlayerCaseInventoryHUD>(HUDWidgetInstance->GetHUDWidgetByType(EVacancyHUDType::CaseInventory));
}

UPlayerToolHUDSuite* AVacancyHUD::GetToolHUDSuite() const
{
	if (!IsValid(HUDWidgetInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("HUDWidgetInstance is not valid in AVacancyHUD."));
		return nullptr;
	}

	return Cast<UPlayerToolHUDSuite>(HUDWidgetInstance->GetHUDWidgetByType(EVacancyHUDType::ToolHUDSuite));
}

void AVacancyHUD::SetHUDPointerRef(EVacancyHUDType HUDType, UVacancyUserWidgetBase* WidgetPtr)
{
	if (HUDType == EVacancyHUDType::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetHUDPointerRef called with HUDType::None. No action taken."));
		return;
	}

	if (!IsValid(WidgetPtr))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetHUDPointerRef called with an invalid WidgetPtr."));
		return;
	}

	switch (HUDType)
	{
		case EVacancyHUDType::ToolHUD:
			ActiveToolHUD = Cast<UPlayerActiveToolHUD>(WidgetPtr);
			break;
		case EVacancyHUDType::CaseInventory:
			CaseInventoryHUD = Cast<UPlayerCaseInventoryHUD>(WidgetPtr);
			break;
		case EVacancyHUDType::ToolHUDSuite:
			ToolHUDSuite = Cast<UPlayerToolHUDSuite>(WidgetPtr);
			break;
		
		default:
			UE_LOG(LogTemp, Warning, TEXT("SetHUDPointerRef called with an unhandled HUDType."));
			break;
	}
}
