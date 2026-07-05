// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/Tools/PlayerToolHUDSuite.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UPlayerToolHUDSuite::OnVacancyWidgetInitialized_Implementation()
{
	Super::OnVacancyWidgetInitialized_Implementation();

	UpdateToolSuiteHUD();
}

void UPlayerToolHUDSuite::OnRefreshVacancyWidget_Implementation()
{
	Super::OnRefreshVacancyWidget_Implementation();

	UpdateToolSuiteHUD();
}

void UPlayerToolHUDSuite::SetToolSlotData(const FVacancyToolHUDSlotData& NewSlotData)
{
	if (NewSlotData.ToolSlot == EVacancyToolHUDSlot::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetToolSlotData failed: ToolSlot is None."));
		return;
	}

	for (FVacancyToolHUDSlotData& ExistingSlot : CurrentToolSlots)
	{
		if (ExistingSlot.ToolSlot == NewSlotData.ToolSlot)
		{
			ExistingSlot = NewSlotData;
			UpdateToolSuiteHUD();
			return;
		}
	}

	CurrentToolSlots.Add(NewSlotData);
	UpdateToolSuiteHUD();
}

void UPlayerToolHUDSuite::SetToolSlotsData(const TArray<FVacancyToolHUDSlotData>& NewToolSlots)
{
	CurrentToolSlots = NewToolSlots;
	UpdateToolSuiteHUD();
}

void UPlayerToolHUDSuite::ClearToolHUDSuite()
{
	CurrentToolSlots.Empty();
	UpdateToolSuiteHUD();
}

void UPlayerToolHUDSuite::UpdateToolSuiteHUD()
{
	if (const FVacancyToolHUDSlotData* EquippedToolData = GetEquippedToolSlotData())
	{
		SetTextBlockText(CurrentToolNameText, EquippedToolData->ToolDisplayName, TEXT("CurrentToolNameText"));
		SetTextBlockText(CurrentToolDetailText, EquippedToolData->ToolDetailText, TEXT("CurrentToolDetailText"));
	}
	else
	{
		SetTextBlockText(CurrentToolNameText, FText::FromString("NO TOOL"), TEXT("CurrentToolNameText"));
		SetTextBlockText(CurrentToolDetailText, FText::FromString("Nothing equipped."), TEXT("CurrentToolDetailText"));
	}

	UpdateFixedToolTexts();
	RebuildToolSlotList();
}

void UPlayerToolHUDSuite::UpdateFixedToolTexts() const
{
	if (const FVacancyToolHUDSlotData* CameraData = FindToolSlotData(EVacancyToolHUDSlot::Camera))
	{
		SetTextBlockText(CameraToolText, BuildToolSlotDisplayText(*CameraData), TEXT("CameraToolText"));
	}

	if (const FVacancyToolHUDSlotData* RecorderData = FindToolSlotData(EVacancyToolHUDSlot::Recorder))
	{
		SetTextBlockText(RecorderToolText, BuildToolSlotDisplayText(*RecorderData), TEXT("RecorderToolText"));
	}

	if (const FVacancyToolHUDSlotData* FlashlightData = FindToolSlotData(EVacancyToolHUDSlot::Flashlight))
	{
		SetTextBlockText(FlashlightToolText, BuildToolSlotDisplayText(*FlashlightData), TEXT("FlashlightToolText"));
	}

	if (const FVacancyToolHUDSlotData* PhoneData = FindToolSlotData(EVacancyToolHUDSlot::Phone))
	{
		SetTextBlockText(PhoneToolText, BuildToolSlotDisplayText(*PhoneData), TEXT("PhoneToolText"));
	}
}

void UPlayerToolHUDSuite::RebuildToolSlotList()
{
	if (!IsValid(ToolSlotListBox))
	{
		return;
	}

	ToolSlotListBox->ClearChildren();

	for (const FVacancyToolHUDSlotData& SlotData : CurrentToolSlots)
	{
		if (!SlotData.bShouldDisplay)
		{
			continue;
		}

		UTextBlock* RowText = NewObject<UTextBlock>(ToolSlotListBox);
		if (!IsValid(RowText))
		{
			continue;
		}

		RowText->SetText(BuildToolSlotDisplayText(SlotData));
		RowText->SetAutoWrapText(false);

		ToolSlotListBox->AddChildToVerticalBox(RowText);
	}
}

FText UPlayerToolHUDSuite::BuildToolSlotDisplayText(const FVacancyToolHUDSlotData& SlotData)
{
	if (!SlotData.bIsUnlocked)
	{
		return FText::Format(
			NSLOCTEXT("VacancyHUD", "ToolSlotLockedFormat", "{0}: LOCKED"),
			SlotData.ToolDisplayName
		);
	}

	if (SlotData.ToolDetailText.IsEmpty())
	{
		return FText::Format(
			NSLOCTEXT("VacancyHUD", "ToolSlotStatusFormat", "{0}: {1}"),
			SlotData.ToolDisplayName,
			SlotData.ToolStatusText
		);
	}

	return FText::Format(
		NSLOCTEXT("VacancyHUD", "ToolSlotFullFormat", "{0}: {1} — {2}"),
		SlotData.ToolDisplayName,
		SlotData.ToolStatusText,
		SlotData.ToolDetailText
	);
}

const FVacancyToolHUDSlotData* UPlayerToolHUDSuite::FindToolSlotData(const EVacancyToolHUDSlot ToolSlot) const
{
	for (const FVacancyToolHUDSlotData& SlotData : CurrentToolSlots)
	{
		if (SlotData.ToolSlot == ToolSlot)
		{
			return &SlotData;
		}
	}

	return nullptr;
}

const FVacancyToolHUDSlotData* UPlayerToolHUDSuite::GetEquippedToolSlotData() const
{
	for (const FVacancyToolHUDSlotData& SlotData : CurrentToolSlots)
	{
		if (SlotData.bIsEquipped)
		{
			return &SlotData;
		}
	}

	return nullptr;
}

void UPlayerToolHUDSuite::SetTextBlockText(
	UTextBlock* TextBlock,
	const FText& NewText,
	const TCHAR* DebugName)
{
	if (!IsValid(TextBlock))
	{
		UE_LOG(LogTemp, Verbose, TEXT("PlayerToolHUDSuite: %s is not bound."), DebugName);
		return;
	}

	if (TextBlock->GetText().EqualTo(NewText))
	{
		return;
	}

	TextBlock->SetText(NewText);
}