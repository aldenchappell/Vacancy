// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Characters/Player/Interaction/InteractionData.h"
#include "InteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class THEAPARTMENT_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void DisplayNewInteraction(const FInteractionInfo& InInteractionInfo);
	void SwitchInteractionVisibility(const bool bVisible);
	
protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock* InteractionText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UImage* InteractionIcon;

private:

	void ClearInteractionDisplay();
	static FName ConvertInteractionEnumToName(const EInteractionType InteractionType);
};
