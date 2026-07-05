// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/Evidence/PlayerCaseInventoryHUD.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UPlayerCaseInventoryHUD::OnVacancyWidgetInitialized_Implementation()
{
	Super::OnVacancyWidgetInitialized_Implementation();

	UpdateCaseInventoryHUD();
}

void UPlayerCaseInventoryHUD::OnRefreshVacancyWidget_Implementation()
{
	Super::OnRefreshVacancyWidget_Implementation();

	UpdateCaseInventoryHUD();
}

void UPlayerCaseInventoryHUD::SetCaseFileData(const FVacancyCaseFileData& NewCaseFileData)
{
	CurrentCaseFileData = NewCaseFileData;
	UpdateCaseInventoryHUD();
}

void UPlayerCaseInventoryHUD::SetEvidenceData(const TArray<FVacancyEvidenceData>& NewEvidenceData)
{
	CurrentEvidenceData = NewEvidenceData;
	UpdateCaseInventoryHUD();
}

void UPlayerCaseInventoryHUD::SetClueSummaryData(const TArray<FVacancyClueSummaryData>& NewClueSummaryData)
{
	CurrentClueSummaryData = NewClueSummaryData;
	UpdateCaseInventoryHUD();
}

void UPlayerCaseInventoryHUD::SetSelectedEvidenceData(const FVacancyEvidenceData& SelectedEvidence)
{
	SetTextBlockText(SelectedEntryTitleText, SelectedEvidence.EvidenceDisplayTitle, TEXT("SelectedEntryTitleText"));
	SetTextBlockText(SelectedEntryDescriptionText, SelectedEvidence.EvidenceDisplayName, TEXT("SelectedEntryDescriptionText"));
}

void UPlayerCaseInventoryHUD::SetSelectedClueData(const FVacancyClueSummaryData& SelectedClue)
{
	SetTextBlockText(SelectedEntryTitleText, FText::FromName(SelectedClue.ClueID), TEXT("SelectedEntryTitleText"));
	SetTextBlockText(SelectedEntryDescriptionText, SelectedClue.ClueDisplaySummaryText, TEXT("SelectedEntryDescriptionText"));
}

void UPlayerCaseInventoryHUD::ClearCaseInventoryHUD()
{
	CurrentCaseFileData = FVacancyCaseFileData();
	CurrentEvidenceData.Empty();
	CurrentClueSummaryData.Empty();

	UpdateCaseInventoryHUD();
}

void UPlayerCaseInventoryHUD::UpdateCaseInventoryHUD()
{
	SetTextBlockText(CaseTitleText, CurrentCaseFileData.CaseDisplayData.CaseDisplayName, TEXT("CaseTitleText"));
	SetTextBlockText(CaseDescriptionText, CurrentCaseFileData.CaseDisplayData.CaseDisplayDescription, TEXT("CaseDescriptionText"));
	SetTextBlockText(CaseLocationText, CurrentCaseFileData.CaseFileDisplayLocation, TEXT("CaseLocationText"));
	SetTextBlockText(CaseIDText, CurrentCaseFileData.CaseDisplayID, TEXT("CaseIDText"));
	SetTextBlockText(CaseStatusText, GetCaseStatusText(CurrentCaseFileData.CaseFileStatus), TEXT("CaseStatusText"));

	SetTextBlockText(SuspectNameText, CurrentCaseFileData.CaseFileSuspectData.SuspectDisplayName, TEXT("SuspectNameText"));

	SetTextBlockText(
		SuspectAgeText,
		FText::AsNumber(CurrentCaseFileData.CaseFileSuspectData.SuspectDisplayAge),
		TEXT("SuspectAgeText")
	);

	SetTextBlockText(
		SuspectOccupationText,
		CurrentCaseFileData.CaseFileSuspectData.SuspectDisplayOccupation,
		TEXT("SuspectOccupationText")
	);

	SetTextBlockText(
		SuspectLastSeenText,
		FText::FromString(CurrentCaseFileData.CaseFileSuspectData.SuspectDisplayLastSeen.ToString(TEXT("%m/%d/%Y"))),
		TEXT("SuspectLastSeenText")
	);

	SetTextBlockText(
		EvidenceCountText,
		FText::Format(NSLOCTEXT("VacancyHUD", "EvidenceCountText", "Evidence: {0}"), CurrentEvidenceData.Num()),
		TEXT("EvidenceCountText")
	);

	SetTextBlockText(
		ClueCountText,
		FText::Format(NSLOCTEXT("VacancyHUD", "ClueCountText", "Clues: {0}"), CurrentClueSummaryData.Num()),
		TEXT("ClueCountText")
	);

	RebuildEvidenceList();
	RebuildClueList();
}

void UPlayerCaseInventoryHUD::RebuildEvidenceList()
{
	if (!IsValid(EvidenceListBox))
	{
		return;
	}

	EvidenceListBox->ClearChildren();

	for (const FVacancyEvidenceData& EvidenceData : CurrentEvidenceData)
	{
		UTextBlock* RowText = NewObject<UTextBlock>(EvidenceListBox);
		if (!IsValid(RowText))
		{
			continue;
		}

		RowText->SetText(EvidenceData.EvidenceDisplayTitle);
		RowText->SetAutoWrapText(true);

		EvidenceListBox->AddChildToVerticalBox(RowText);
	}
}

void UPlayerCaseInventoryHUD::RebuildClueList()
{
	if (!IsValid(ClueListBox))
	{
		return;
	}

	ClueListBox->ClearChildren();

	for (const FVacancyClueSummaryData& ClueData : CurrentClueSummaryData)
	{
		UTextBlock* RowText = NewObject<UTextBlock>(ClueListBox);
		if (!IsValid(RowText))
		{
			continue;
		}

		const FText RowDisplayText = FText::Format(
			NSLOCTEXT("VacancyHUD", "ClueRowFormat", "[{0}] {1}"),
			GetClueTierText(ClueData.ClueImportance),
			ClueData.ClueDisplaySummaryText
		);

		RowText->SetText(RowDisplayText);
		RowText->SetAutoWrapText(true);

		ClueListBox->AddChildToVerticalBox(RowText);
	}
}

void UPlayerCaseInventoryHUD::SetTextBlockText(
	UTextBlock* TextBlock,
	const FText& NewText,
	const TCHAR* DebugName)
{
	if (!IsValid(TextBlock))
	{
		UE_LOG(LogTemp, Verbose, TEXT("PlayerCaseInventoryHUD: %s is not bound."), DebugName);
		return;
	}

	if (TextBlock->GetText().EqualTo(NewText))
	{
		return;
	}

	TextBlock->SetText(NewText);
}

FText UPlayerCaseInventoryHUD::GetCaseStatusText(const EVacancyCaseFileStatus Status)
{
	switch (Status)
	{
	case EVacancyCaseFileStatus::Open:
		return FText::FromString("OPEN");

	case EVacancyCaseFileStatus::Closed:
		return FText::FromString("CLOSED");

	default:
		return FText::FromString("UNKNOWN");
	}
}

FText UPlayerCaseInventoryHUD::GetClueTierText(const EVacancyClueTier Tier)
{
	switch (Tier)
	{
	case EVacancyClueTier::Low:
		return FText::FromString("LOW");

	case EVacancyClueTier::Medium:
		return FText::FromString("MED");

	case EVacancyClueTier::High:
		return FText::FromString("HIGH");

	default:
		return FText::FromString("UNK");
	}
}