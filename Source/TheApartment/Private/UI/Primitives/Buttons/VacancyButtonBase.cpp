// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Primitives/Buttons/VacancyButtonBase.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

UVacancyButtonBase::UVacancyButtonBase()
{
	BindButtonEvents();
}

void UVacancyButtonBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	BindButtonEvents();

	SetIsEnabled(!bStartDisabled);
}

void UVacancyButtonBase::BeginDestroy()
{
	UnbindButtonEvents();

	Super::BeginDestroy();
}

void UVacancyButtonBase::BindButtonEvents()
{
	if (bEventsBound)
	{
		return;
	}

	OnClicked.AddUniqueDynamic(this, &UVacancyButtonBase::HandleButtonClicked);
	OnHovered.AddUniqueDynamic(this, &UVacancyButtonBase::HandleButtonHovered);
	OnUnhovered.AddUniqueDynamic(this, &UVacancyButtonBase::HandleButtonUnhovered);
	OnPressed.AddUniqueDynamic(this, &UVacancyButtonBase::HandleButtonPressed);
	OnReleased.AddUniqueDynamic(this, &UVacancyButtonBase::HandleButtonReleased);

	bEventsBound = true;
}

void UVacancyButtonBase::UnbindButtonEvents()
{
	if (!bEventsBound)
	{
		return;
	}

	OnClicked.RemoveDynamic(this, &UVacancyButtonBase::HandleButtonClicked);
	OnHovered.RemoveDynamic(this, &UVacancyButtonBase::HandleButtonHovered);
	OnUnhovered.RemoveDynamic(this, &UVacancyButtonBase::HandleButtonUnhovered);
	OnPressed.RemoveDynamic(this, &UVacancyButtonBase::HandleButtonPressed);
	OnReleased.RemoveDynamic(this, &UVacancyButtonBase::HandleButtonReleased);

	bEventsBound = false;
}

void UVacancyButtonBase::SetButtonID(const FName NewButtonID)
{
	ButtonID = NewButtonID;
}

void UVacancyButtonBase::SetButtonInteractable(const bool bInteractable)
{
	SetIsEnabled(bInteractable);
}

bool UVacancyButtonBase::IsButtonInteractable() const
{
	return GetIsEnabled();
}

void UVacancyButtonBase::SetButtonSelected(const bool bNewSelected)
{
	if (bIsSelected == bNewSelected)
	{
		return;
	}

	bIsSelected = bNewSelected;
	OnButtonSelectedChanged(bIsSelected);
}

void UVacancyButtonBase::HandleButtonClicked()
{
	if (bLogButtonEvents)
	{
		UE_LOG(LogTemp, Log, TEXT("Vacancy button clicked: %s | ButtonID: %s"), *GetNameSafe(this), *ButtonID.ToString());
	}

	PlayButtonSound(ClickSound);

	OnButtonClicked();
	OnVacancyButtonClicked.Broadcast(this);

	if (bDisableAfterClick)
	{
		SetButtonInteractable(false);
	}
}

void UVacancyButtonBase::HandleButtonHovered()
{
	if (bLogButtonEvents)
	{
		UE_LOG(LogTemp, Log, TEXT("Vacancy button hovered: %s | ButtonID: %s"), *GetNameSafe(this), *ButtonID.ToString());
	}

	PlayButtonSound(HoverSound);

	OnButtonHovered();
	OnVacancyButtonHovered.Broadcast(this);
}

void UVacancyButtonBase::HandleButtonUnhovered()
{
	OnButtonUnhovered();
	OnVacancyButtonUnhovered.Broadcast(this);
}

void UVacancyButtonBase::HandleButtonPressed()
{
	OnButtonPressed();
	OnVacancyButtonPressed.Broadcast(this);
}

void UVacancyButtonBase::HandleButtonReleased()
{
	OnButtonReleased();
	OnVacancyButtonReleased.Broadcast(this);
}

void UVacancyButtonBase::PlayButtonSound(USoundBase* Sound) const
{
	if (!IsValid(Sound))
	{
		return;
	}

	UGameplayStatics::PlaySound2D(this, Sound);
}

void UVacancyButtonBase::OnButtonClicked_Implementation()
{
}

void UVacancyButtonBase::OnButtonHovered_Implementation()
{
}

void UVacancyButtonBase::OnButtonUnhovered_Implementation()
{
}

void UVacancyButtonBase::OnButtonPressed_Implementation()
{
}

void UVacancyButtonBase::OnButtonReleased_Implementation()
{
}

void UVacancyButtonBase::OnButtonSelectedChanged_Implementation(bool bSelected)
{
}