// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Investigation/Clues/VacancyCaseData.h"
#include "UI/VacancyUserWidgetBase.h"
#include "CaseClueHUDWidget.generated.h"

class UImage;
class UTextBlock;

/**
 * Small contextual clue/case notification widget.
 *
 * Intended use:
 * - Show "CASE CLUE" updates.
 * - Show location-specific clue text.
 * - Show clue importance through a small colored dot.
 *
 * Data source:
 * - Uses FVacancyClueSummaryData from VacancyCaseData.h.
 *
 * UMG optional bind names:
 * - ClueHeaderDotImage
 * - ClueHeaderText
 * - ClueLocationText
 * - ClueDescriptionText
 */
UCLASS()
class THEAPARTMENT_API UCaseClueHUDWidget : public UVacancyUserWidgetBase
{
	GENERATED_BODY()

public:

	/**
	 * Updates the HUD using existing clue summary data.
	 */
	UFUNCTION(BlueprintCallable, Category = "Case Clue HUD")
	void SetClueSummaryData(const FVacancyClueSummaryData& NewClueData, const FText& NewLocationText);

	/**
	 * Convenience setter for quick clue updates without building a struct first.
	 */
	UFUNCTION(BlueprintCallable, Category = "Case Clue HUD")
	void SetClueInfo(const FName& ClueID, const FText& LocationText, const FText& DescriptionText, EVacancyClueTier ClueTier);

	/**
	 * Clears the active clue display.
	 */
	UFUNCTION(BlueprintCallable, Category = "Case Clue HUD")
	void ClearClueHUD();

	/**
	 * Re-applies the currently stored clue data to the bound UMG widgets.
	 */
	UFUNCTION(BlueprintCallable, Category = "Case Clue HUD")
	void UpdateClueHUD();

	UFUNCTION(BlueprintPure, Category = "Case Clue HUD")
	bool HasActiveClue() const { return bHasActiveClue; }

protected:

	virtual void OnVacancyWidgetInitialized_Implementation() override;
	virtual void OnRefreshVacancyWidget_Implementation() override;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> ClueHeaderDotImage = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ClueHeaderText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ClueLocationText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ClueDescriptionText = nullptr;

	/**
	 * When true, this widget collapses itself when no clue is active.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Case Clue HUD")
	bool bHideWhenNoActiveClue = true;

	/**
	 * Header shown above clue data.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Case Clue HUD")
	FText DefaultHeaderText = FText::FromString("CASE CLUE");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Case Clue HUD")
	FLinearColor DefaultHeaderDotColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Case Clue HUD")
	FLinearColor LowTierClueColor = FLinearColor::Gray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Case Clue HUD")
	FLinearColor MediumTierClueColor = FLinearColor::Yellow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Case Clue HUD")
	FLinearColor HighTierClueColor = FLinearColor::Red;

	
private:
	static void SetTextBlockText(UTextBlock* TextBlock, const FText& NewText, const TCHAR* DebugName);
	FLinearColor GetColorForClueTier(EVacancyClueTier ClueTier) const;

	UPROPERTY(VisibleInstanceOnly, Category = "Case Clue HUD")
	FVacancyClueSummaryData CurrentClueData;

	UPROPERTY(VisibleInstanceOnly, Category = "Case Clue HUD")
	FText CurrentLocationText = FText::GetEmpty();

	bool bHasActiveClue = false;
};