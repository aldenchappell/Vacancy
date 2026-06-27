// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/Gameplay/VacancyUIUtils.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

void UVacancyUIUtils::ShowWidget(UUserWidget* Widget, const bool bShow)
{
	if (!IsValid(Widget))
	{
		UE_LOG(LogTemp, Warning, TEXT("ShowWidget called with null Widget."));
		return;
	}

	Widget->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UVacancyUIUtils::SetWidgetVisibility(UUserWidget* Widget, const ESlateVisibility Visibility)
{
	if (!IsValid(Widget))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetWidgetVisibility called with null Widget."));
		return;
	}

	Widget->SetVisibility(Visibility);
}

void UVacancyUIUtils::SetWidgetText(UTextBlock* TextBlock, const FText& NewText)
{
	if (!IsValid(TextBlock))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetWidgetTextColor called with null TextBlock."));
		return;
	}

	if (NewText.EqualTo(TextBlock->GetText()))
	{
		return; // No change needed, already has the desired text
	}
	
	TextBlock->SetText(NewText);
}

void UVacancyUIUtils::SetWidgetTextColor(UTextBlock* TextBlock, const FSlateColor& NewColor)
{
	if (!IsValid(TextBlock))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetWidgetTextColor called with null TextBlock."));
		return;
	}

	if (NewColor == TextBlock->GetColorAndOpacity())
	{
		return; // No change needed, already in the desired color
	}

	TextBlock->SetColorAndOpacity(NewColor);
}
