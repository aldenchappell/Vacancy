// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/VacancyUserWidgetBase.h"
#include "PlayerToolHUDSuite.generated.h"

class UTextBlock;
class UVerticalBox;
class UTexture2D;

UENUM(BlueprintType)
enum class EVacancyToolHUDSlot : uint8
{
	None,
	Camera,
	Recorder,
	Flashlight,
	Phone,
	Generic
};

/**
 * Local display-only data for one tool HUD slot.
 *
 * This is intentionally stored here instead of in VacancyCaseData.h because
 * tools are not case/clue/objective data.
 */
USTRUCT(BlueprintType)
struct FVacancyToolHUDSlotData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool HUD")
	EVacancyToolHUDSlot ToolSlot = EVacancyToolHUDSlot::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool HUD")
	FName ToolID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool HUD")
	FText ToolDisplayName = FText::FromString("Tool");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool HUD")
	FText ToolStatusText = FText::FromString("AVAILABLE");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool HUD")
	FText ToolDetailText = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool HUD")
	TObjectPtr<UTexture2D> ToolIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool HUD")
	bool bIsUnlocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool HUD")
	bool bIsEquipped = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tool HUD")
	bool bShouldDisplay = true;
};

/**
 * Displays all player tool states.
 *
 * Intended use:
 * - Camera state.
 * - Recorder state.
 * - Flashlight state.
 * - Phone state.
 * - Future tool states.
 *
 * Widget bind names:
 * - CurrentToolNameText
 * - CurrentToolDetailText
 * - CameraToolText
 * - RecorderToolText
 * - FlashlightToolText
 * - PhoneToolText
 * - ToolSlotListBox
 */
UCLASS()
class THEAPARTMENT_API UPlayerToolHUDSuite : public UVacancyUserWidgetBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Tool HUD")
	void SetToolSlotData(const FVacancyToolHUDSlotData& NewSlotData);

	UFUNCTION(BlueprintCallable, Category = "Tool HUD")
	void SetToolSlotsData(const TArray<FVacancyToolHUDSlotData>& NewToolSlots);

	UFUNCTION(BlueprintCallable, Category = "Tool HUD")
	void ClearToolHUDSuite();

	UFUNCTION(BlueprintCallable, Category = "Tool HUD")
	void UpdateToolSuiteHUD();

protected:

	virtual void OnVacancyWidgetInitialized_Implementation() override;
	virtual void OnRefreshVacancyWidget_Implementation() override;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> CurrentToolNameText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> CurrentToolDetailText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> CameraToolText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> RecorderToolText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> FlashlightToolText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> PhoneToolText = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UVerticalBox> ToolSlotListBox = nullptr;

private:
	static void SetTextBlockText(UTextBlock* TextBlock, const FText& NewText, const TCHAR* DebugName);
	void UpdateFixedToolTexts() const;
	void RebuildToolSlotList();

	static FText BuildToolSlotDisplayText(const FVacancyToolHUDSlotData& SlotData);
	const FVacancyToolHUDSlotData* FindToolSlotData(EVacancyToolHUDSlot ToolSlot) const;
	const FVacancyToolHUDSlotData* GetEquippedToolSlotData() const;

	UPROPERTY(VisibleInstanceOnly, Category = "Tool HUD")
	TArray<FVacancyToolHUDSlotData> CurrentToolSlots;
};