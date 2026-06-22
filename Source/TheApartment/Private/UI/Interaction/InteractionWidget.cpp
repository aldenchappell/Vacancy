// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Interaction/InteractionWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"

UInteractionWidget::UInteractionWidget(const FObjectInitializer& ObjectInitializer)
{
	InteractionIcon = ObjectInitializer.CreateDefaultSubobject<UImage>(this, TEXT("InteractionIcon"));
	InteractionIcon->SetBrushFromTexture(nullptr);
	InteractionText = ObjectInitializer.CreateDefaultSubobject<UTextBlock>(this, TEXT("InteractionText"));
	InteractionText->SetText(FText::GetEmpty());

	
}


void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ClearInteractionDisplay();
}

void UInteractionWidget::NativeDestruct()
{
	Super::NativeDestruct();
	ClearInteractionDisplay();
}

void UInteractionWidget::SwitchInteractionVisibility(const bool bVisible)
{
	const ESlateVisibility NewVisibility = bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	SetVisibility(NewVisibility);
}

void UInteractionWidget::ClearInteractionDisplay()
{
	SwitchInteractionVisibility(false);
	InteractionIcon->SetBrushFromTexture(nullptr);
	InteractionText->SetText(FText::GetEmpty());
}

void UInteractionWidget::DisplayNewInteraction(const FInteractionVisualInfo& InteractionVisualInfo)
{
	ClearInteractionDisplay();
	SwitchInteractionVisibility(true);

	if (InteractionVisualInfo.InteractionText.IsEmpty())
	{
		return; // No text to display, so exit early
	}

	if (InteractionVisualInfo.InteractionIcon == nullptr)
	{
		return; // No icon to display, so exit early
	}

	InteractionIcon->SetBrushFromTexture(InteractionVisualInfo.InteractionIcon);
	InteractionText->SetText(InteractionVisualInfo.InteractionText);
}
