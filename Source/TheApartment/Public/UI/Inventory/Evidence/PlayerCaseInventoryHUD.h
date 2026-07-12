// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Investigation/Clues/VacancyCaseData.h"
#include "UI/VacancyUserWidgetBase.h"
#include "PlayerCaseInventoryHUD.generated.h"

class UTextBlock;
class UVerticalBox;

/**
 * Full case inventory / case file HUD.
 *
 * Intended use:
 * - Display case name, case ID, location, status, and summary.
 * - Display evidence entries.
 * - Display clue summaries.
 * - Display selected evidence/clue details.
 *
 * Data source:
 * - FVacancyCaseFileData
 * - FVacancyEvidenceData
 * - FVacancyClueSummaryData
 *
 * Widget bind names:
 * - CaseTitleText
 * - CaseDescriptionText
 * - CaseLocationText
 * - CaseIDText
 * - CaseStatusText
 * - SuspectNameText
 * - SuspectAgeText
 * - SuspectOccupationText
 * - SuspectLastSeenText
 * - EvidenceCountText
 * - ClueCountText
 * - SelectedEntryTitleText
 * - SelectedEntryDescriptionText
 * - EvidenceListBox
 * - ClueListBox
 */
UCLASS()
class THEAPARTMENT_API UPlayerCaseInventoryHUD : public UVacancyUserWidgetBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Case Inventory HUD")
	void SetCaseFileData(const FVacancyCaseFileData& NewCaseFileData);

	UFUNCTION(BlueprintCallable, Category = "Case Inventory HUD")
	void SetEvidenceData(const TArray<FVacancyEvidenceData>& NewEvidenceData);

	UFUNCTION(BlueprintCallable, Category = "Case Inventory HUD")
	void SetClueSummaryData(const TArray<FVacancyClueSummaryData>& NewClueSummaryData);

	UFUNCTION(BlueprintCallable, Category = "Case Inventory HUD")
	void SetSelectedEvidenceData(const FVacancyEvidenceData& SelectedEvidence) const;

	UFUNCTION(BlueprintCallable, Category = "Case Inventory HUD")
	void SetSelectedClueData(const FVacancyClueSummaryData& SelectedClue) const;

	UFUNCTION(BlueprintCallable, Category = "Case Inventory HUD")
	void ClearCaseInventoryHUD();

	UFUNCTION(BlueprintCallable, Category = "Case Inventory HUD")
	void UpdateCaseInventoryHUD();

protected:

	virtual void OnVacancyWidgetInitialized_Implementation() override;
	virtual void OnRefreshVacancyWidget_Implementation() override;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> CaseTitleText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> CaseDescriptionText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> CaseLocationText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> CaseIDText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> CaseStatusText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> SuspectNameText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> SuspectAgeText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> SuspectOccupationText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> SuspectLastSeenText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> EvidenceCountText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ClueCountText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> SelectedEntryTitleText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> SelectedEntryDescriptionText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UVerticalBox> EvidenceListBox = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UVerticalBox> ClueListBox = nullptr;

private:
	static void SetTextBlockText(UTextBlock* TextBlock, const FText& NewText, const TCHAR* DebugName);
	void RebuildEvidenceList();
	void RebuildClueList();

	static FText GetCaseStatusText(EVacancyCaseFileStatus Status);
	static FText GetClueTierText(EVacancyClueTier Tier);

	UPROPERTY(VisibleInstanceOnly, Category = "Case Inventory HUD")
	FVacancyCaseFileData CurrentCaseFileData;

	UPROPERTY(VisibleInstanceOnly, Category = "Case Inventory HUD")
	TArray<FVacancyEvidenceData> CurrentEvidenceData;

	UPROPERTY(VisibleInstanceOnly, Category = "Case Inventory HUD")
	TArray<FVacancyClueSummaryData> CurrentClueSummaryData;
};