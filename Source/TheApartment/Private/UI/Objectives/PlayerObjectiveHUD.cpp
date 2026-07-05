// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Objectives/PlayerObjectiveHUD.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Systems/Investigation/Objectives/BaseVacancyCaseObjective.h"

void UPlayerObjectiveHUD::OnVacancyWidgetInitialized_Implementation()
{
	Super::OnVacancyWidgetInitialized_Implementation();

	UpdateObjectiveHUD();
}

void UPlayerObjectiveHUD::OnRefreshVacancyWidget_Implementation()
{
	Super::OnRefreshVacancyWidget_Implementation();

	UpdateObjectiveHUD();
}

void UPlayerObjectiveHUD::SetObjectiveData(
	const FVacancyCaseObjectiveData& NewObjectiveData,
	const EVacancyCaseObjectiveStatus NewObjectiveStatus)
{
	CurrentObjectiveData = NewObjectiveData;
	CurrentObjectiveStatus = NewObjectiveStatus;
	bHasObjectiveData = true;

	UpdateObjectiveHUD();
}

void UPlayerObjectiveHUD::SetObjectiveFromObject(const UBaseVacancyCaseObjective* Objective)
{
	if (!IsValid(Objective))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetObjectiveFromObject failed: Objective is invalid."));
		return;
	}

	SetObjectiveData(Objective->GetObjectiveData(), Objective->GetObjectiveStatus());
}

void UPlayerObjectiveHUD::SetObjectiveStatus(const EVacancyCaseObjectiveStatus NewObjectiveStatus)
{
	CurrentObjectiveStatus = NewObjectiveStatus;

	UpdateObjectiveHUD();
}

void UPlayerObjectiveHUD::ClearObjectiveHUD()
{
	CurrentObjectiveData = FVacancyCaseObjectiveData();
	CurrentObjectiveStatus = EVacancyCaseObjectiveStatus::MAX;
	bHasObjectiveData = false;

	UpdateObjectiveHUD();
}

void UPlayerObjectiveHUD::UpdateObjectiveHUD()
{
	if (!bHasObjectiveData && bHideWhenNoObjective)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	SetVisibility(ESlateVisibility::Visible);

	SetTextBlockText(ObjectiveHeaderText, DefaultHeaderText, TEXT("ObjectiveHeaderText"));
	SetTextBlockText(ActiveObjectiveText, GetActiveObjectiveDisplayText(), TEXT("ActiveObjectiveText"));
	SetTextBlockText(ObjectiveProgressText, GetProgressText(), TEXT("ObjectiveProgressText"));
	SetTextBlockText(ObjectiveStatusText, GetObjectiveStatusText(CurrentObjectiveStatus), TEXT("ObjectiveStatusText"));

	RebuildObjectiveList();
}

void UPlayerObjectiveHUD::RebuildObjectiveList()
{
	if (!IsValid(ObjectiveListBox))
	{
		return;
	}

	ObjectiveListBox->ClearChildren();

	for (const FVacancyCaseObjectiveStateData& ObjectiveStateData : CurrentObjectiveData.Objectives)
	{
		UTextBlock* RowText = NewObject<UTextBlock>(ObjectiveListBox);
		if (!IsValid(RowText))
		{
			continue;
		}

		const FString Prefix = ObjectiveStateData.bIsOptional ? TEXT("○") : TEXT("•");

		const FText RowDisplayText = FText::Format(
			NSLOCTEXT("VacancyHUD", "ObjectiveStateRowFormat", "{0} {1}"),
			FText::FromString(Prefix),
			ObjectiveStateData.ObjectiveDisplayText
		);

		RowText->SetText(RowDisplayText);
		RowText->SetAutoWrapText(true);

		ObjectiveListBox->AddChildToVerticalBox(RowText);
	}
}

FText UPlayerObjectiveHUD::GetActiveObjectiveDisplayText() const
{
	if (!bHasObjectiveData)
	{
		return FText::FromString("No active objective.");
	}

	if (CurrentObjectiveData.Objectives.Num() > 0)
	{
		return CurrentObjectiveData.Objectives[0].ObjectiveDisplayText;
	}

	if (!CurrentObjectiveData.ObjectiveID.IsNone())
	{
		return FText::FromName(CurrentObjectiveData.ObjectiveID);
	}

	return FText::FromString("No objective text.");
}

FText UPlayerObjectiveHUD::GetProgressText() const
{
	if (!bHasObjectiveData)
	{
		return FText::GetEmpty();
	}

	return FText::Format(
		NSLOCTEXT("VacancyHUD", "ObjectiveProgressFormat", "{0} step(s)"),
		CurrentObjectiveData.Objectives.Num()
	);
}

FText UPlayerObjectiveHUD::GetObjectiveStatusText(const EVacancyCaseObjectiveStatus Status)
{
	switch (Status)
	{
		case EVacancyCaseObjectiveStatus::Active:
			return FText::FromString("ACTIVE");

		case EVacancyCaseObjectiveStatus::Completed:
			return FText::FromString("COMPLETED");

		case EVacancyCaseObjectiveStatus::Failed:
			return FText::FromString("FAILED");

		default:
			return FText::FromString("INACTIVE");
	}
}

void UPlayerObjectiveHUD::SetTextBlockText(
	UTextBlock* TextBlock,
	const FText& NewText,
	const TCHAR* DebugName)
{
	if (!IsValid(TextBlock))
	{
		UE_LOG(LogTemp, Verbose, TEXT("PlayerObjectiveHUD: %s is not bound."), DebugName);
		return;
	}

	if (TextBlock->GetText().EqualTo(NewText))
	{
		return;
	}

	TextBlock->SetText(NewText);
}