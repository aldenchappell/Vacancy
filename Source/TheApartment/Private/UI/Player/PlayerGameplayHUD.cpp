// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/PlayerGameplayHUD.h"

#include "UI/VacancyHUDData.h"

void UPlayerGameplayHUD::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeHUDWidgets();
}

void UPlayerGameplayHUD::NativeDestruct()
{
	Super::NativeDestruct();
}

void UPlayerGameplayHUD::InitializeHUDWidgets()
{
	if (HUDWidgetsToInitialize.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("InitializeHUDWidgets called but HudWidgetsToInitialize is empty."));
		return;
	}

	for (const TSubclassOf<UUserWidget>& WidgetClass : HUDWidgetsToInitialize)
	{
		if (!IsValid(WidgetClass))
		{
			UE_LOG(LogTemp, Warning, TEXT("InitializeHUDWidgets: WidgetClass is null."));
			continue;
		}

		UVacancyUserWidgetBase* NewWidget = CreateWidget<UVacancyUserWidgetBase>(GetWorld(), WidgetClass);
		if (!IsValid(NewWidget))
		{
			UE_LOG(LogTemp, Warning, TEXT("InitializeHUDWidgets: Failed to create widget of class %s."), *GetNameSafe(WidgetClass));
			continue;
		}

		//init the widget and add it to the viewport
		NewWidget->InitializeVacancyWidget();
		NewWidget->AddToViewport();

		//show or hide the widget based on the bAutoShowWidgetOnConstruction property in the widget's data
		const bool bAutoShowWidget = NewWidget->GetWidgetData().bAutoShowWidgetOnConstruction;
		NewWidget->SetVisibility(bAutoShowWidget ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}
