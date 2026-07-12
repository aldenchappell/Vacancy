// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Investigation/Clues/VacancyCaseData.h"
#include "UI/VacancyUserWidgetBase.h"
#include "PlayerObjectiveHUD.generated.h"

class UTextBlock;
class UVerticalBox;
class UBaseVacancyCaseObjective;

/**
 * Player objective HUD.
 *
 * Intended use:
 * - Show current objective.
 * - Show objective status.
 * - Show objective sub-entries from FVacancyCaseObjectiveData.Objectives.
 *
 * Data source:
 * - FVacancyCaseObjectiveData from VacancyCaseData.h.
 * - EVacancyCaseObjectiveStatus from VacancyCaseData.h.
 *
 * UMG optional bind names:
 * - ObjectiveHeaderText
 * - ActiveObjectiveText
 * - ObjectiveProgressText
 * - ObjectiveStatusText
 * - ObjectiveListBox
 */
UCLASS()
class THEAPARTMENT_API UPlayerObjectiveHUD : public UVacancyUserWidgetBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Objective HUD")
	void SetObjectiveData(const FVacancyCaseObjectiveData& NewObjectiveData, const EVacancyCaseObjectiveStatus NewObjectiveStatus);

	UFUNCTION(BlueprintCallable, Category = "Objective HUD")
	void SetObjectiveFromObject(const UBaseVacancyCaseObjective* Objective);

	UFUNCTION(BlueprintCallable, Category = "Objective HUD")
	void SetObjectiveStatus(const EVacancyCaseObjectiveStatus NewObjectiveStatus);

	UFUNCTION(BlueprintCallable, Category = "Objective HUD")
	void ClearObjectiveHUD();

	UFUNCTION(BlueprintCallable, Category = "Objective HUD")
	void UpdateObjectiveHUD();

protected:

	virtual void OnVacancyWidgetInitialized_Implementation() override;
	virtual void OnRefreshVacancyWidget_Implementation() override;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ObjectiveHeaderText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ActiveObjectiveText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ObjectiveProgressText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ObjectiveStatusText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UVerticalBox> ObjectiveListBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objective HUD")
	FText DefaultHeaderText = FText::FromString("OBJECTIVE");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objective HUD")
	bool bHideWhenNoObjective = false;

private:
	static void SetTextBlockText(UTextBlock* TextBlock, const FText& NewText, const TCHAR* DebugName);
	void RebuildObjectiveList();

	static FText GetObjectiveStatusText(const EVacancyCaseObjectiveStatus Status);
	FText GetActiveObjectiveDisplayText() const;
	FText GetProgressText() const;

	UPROPERTY(VisibleInstanceOnly, Category = "Objective HUD")
	FVacancyCaseObjectiveData CurrentObjectiveData;

	UPROPERTY(VisibleInstanceOnly, Category = "Objective HUD")
	EVacancyCaseObjectiveStatus CurrentObjectiveStatus = EVacancyCaseObjectiveStatus::MAX;

	bool bHasObjectiveData = false;
};