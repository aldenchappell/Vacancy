// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Interaction/InteractionWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Components/Characters/Player/Interaction/InteractableInterface.h"

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

FName UInteractionWidget::ConvertInteractionEnumToName(const EInteractionType InteractionType)
{
	if (InteractionType == EInteractionType::None)
	{
		return "Interact";
	}
	
	return FName(*UEnum::GetValueAsString(InteractionType));
}

void UInteractionWidget::DisplayNewInteraction(const FInteractionInfo& InInteractionInfo)
{
	ClearInteractionDisplay();
	SwitchInteractionVisibility(true);

	if (InInteractionInfo.InteractionVisualInfo.InteractionIcon == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("DisplayNewInteraction called with null InteractionIcon in InteractionInfo."));
		return; // No icon to display, so exit early
	}
	
	const AActor* Interactable = InInteractionInfo.InteractionBasicInfo.InteractableActor;
	if (!IsValid(Interactable))
	{
		UE_LOG(LogTemp, Warning, TEXT("DisplayNewInteraction called with null InteractableActor in InteractionInfo."));
		return; // If the interactable actor is null, exit early
	}

	const FInteractionInfo InteractionInfo = IInteractableInterface::Execute_GetInteractionInfo(Interactable);
	
	const FName InteractionName = ConvertInteractionEnumToName(InteractionInfo.InteractionBasicInfo.InteractionType);
	InteractionText->SetText(FText::FromName(InteractionName));
	InteractionIcon->SetBrushFromTexture(InInteractionInfo.InteractionVisualInfo.InteractionIcon);
}
