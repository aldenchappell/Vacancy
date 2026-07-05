// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Case/CaseClueHUDWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCaseClueHUDWidget::OnVacancyWidgetInitialized_Implementation()
{
	Super::OnVacancyWidgetInitialized_Implementation();

	UpdateClueHUD();
}

void UCaseClueHUDWidget::OnRefreshVacancyWidget_Implementation()
{
	Super::OnRefreshVacancyWidget_Implementation();

	UpdateClueHUD();
}

void UCaseClueHUDWidget::SetClueSummaryData(
	const FVacancyClueSummaryData& NewClueData,
	const FText& NewLocationText)
{
	CurrentClueData = NewClueData;
	CurrentLocationText = NewLocationText;
	bHasActiveClue = true;

	UpdateClueHUD();
}

void UCaseClueHUDWidget::SetClueInfo(
	const FName& ClueID,
	const FText& LocationText,
	const FText& DescriptionText,
	const EVacancyClueTier ClueTier)
{
	FVacancyClueSummaryData NewClueData;
	NewClueData.ClueID = ClueID;
	NewClueData.ClueDisplaySummaryText = DescriptionText;
	NewClueData.ClueImportance = ClueTier;

	SetClueSummaryData(NewClueData, LocationText);
}

void UCaseClueHUDWidget::ClearClueHUD()
{
	CurrentClueData = FVacancyClueSummaryData();
	CurrentLocationText = FText::GetEmpty();
	bHasActiveClue = false;

	UpdateClueHUD();
}

void UCaseClueHUDWidget::UpdateClueHUD()
{
	if (!bHasActiveClue && bHideWhenNoActiveClue)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	SetVisibility(ESlateVisibility::Visible);

	if (IsValid(ClueHeaderDotImage))
	{
		ClueHeaderDotImage->SetVisibility(bHasActiveClue ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		ClueHeaderDotImage->SetColorAndOpacity(GetColorForClueTier(CurrentClueData.ClueImportance));
	}

	SetTextBlockText(ClueHeaderText, DefaultHeaderText, TEXT("ClueHeaderText"));
	SetTextBlockText(ClueLocationText, CurrentLocationText, TEXT("ClueLocationText"));
	SetTextBlockText(ClueDescriptionText, CurrentClueData.ClueDisplaySummaryText, TEXT("ClueDescriptionText"));
}

void UCaseClueHUDWidget::SetTextBlockText(
	UTextBlock* TextBlock,
	const FText& NewText,
	const TCHAR* DebugName)
{
	if (!IsValid(TextBlock))
	{
		UE_LOG(LogTemp, Verbose, TEXT("CaseClueHUDWidget: %s is not bound."), DebugName);
		return;
	}

	if (TextBlock->GetText().EqualTo(NewText))
	{
		return;
	}

	TextBlock->SetText(NewText);
}

FLinearColor UCaseClueHUDWidget::GetColorForClueTier(const EVacancyClueTier ClueTier) const
{
	switch (ClueTier)
	{
		case EVacancyClueTier::Low:
			return LowTierClueColor;

		case EVacancyClueTier::Medium:
			return MediumTierClueColor;

		case EVacancyClueTier::High:
			return HighTierClueColor;

		default:
			return FLinearColor::White;
	}
}