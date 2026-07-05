// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menus/PauseMenu/PauseHUD.h"

#include "UI/Primitives/Buttons/VacancyButtonBase.h"
#include "Utilities/Gameplay/VacancyMenuUtils.h"

namespace VacancyPauseButtonIDs
{
	static const FName Resume(TEXT("Resume"));
	static const FName Settings(TEXT("Settings"));
	static const FName MainMenu(TEXT("MainMenu"));
	static const FName Exit(TEXT("Exit"));
}

void UPauseHUD::NativeDestruct()
{
	UnbindPauseButtons();

	Super::NativeDestruct();
}

void UPauseHUD::OnVacancyWidgetInitialized_Implementation()
{
	Super::OnVacancyWidgetInitialized_Implementation();

	ConfigurePauseButtons();
	BindPauseButtons();
}

void UPauseHUD::OnToggleVacancyWidget_Implementation(const bool bVisible)
{
	Super::OnToggleVacancyWidget_Implementation(bVisible);

	if (!bVisible)
	{
		return;
	}

	SetAllPauseButtonsInteractable(true);

	if (bPauseGameWhenShown)
	{
		UVacancyMenuUtils::SetGamePaused(this, true);
	}

	UVacancyMenuUtils::ApplyUIOnlyInputMode(this, this);

	if (bFocusResumeButtonWhenShown && IsValid(ResumeButton))
	{
		ResumeButton->SetKeyboardFocus();
	}
}

void UPauseHUD::OnRefreshVacancyWidget_Implementation()
{
	Super::OnRefreshVacancyWidget_Implementation();

	SetAllPauseButtonsInteractable(true);
}

void UPauseHUD::ConfigurePauseButtons()
{
	if (IsValid(ResumeButton) && ResumeButton->GetButtonID().IsNone())
	{
		ResumeButton->SetButtonID(VacancyPauseButtonIDs::Resume);
	}

	if (IsValid(SettingsButton) && SettingsButton->GetButtonID().IsNone())
	{
		SettingsButton->SetButtonID(VacancyPauseButtonIDs::Settings);
	}

	if (IsValid(MainMenuButton) && MainMenuButton->GetButtonID().IsNone())
	{
		MainMenuButton->SetButtonID(VacancyPauseButtonIDs::MainMenu);
	}

	if (IsValid(ExitButton) && ExitButton->GetButtonID().IsNone())
	{
		ExitButton->SetButtonID(VacancyPauseButtonIDs::Exit);
	}
}

void UPauseHUD::BindPauseButtons()
{
	if (bButtonsBound)
	{
		return;
	}

	if (IsValid(ResumeButton))
	{
		ResumeButton->OnVacancyButtonClicked.AddUniqueDynamic(this, &UPauseHUD::HandleResumeButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PauseHUD: ResumeButton is not bound."));
	}

	if (IsValid(SettingsButton))
	{
		SettingsButton->OnVacancyButtonClicked.AddUniqueDynamic(this, &UPauseHUD::HandleSettingsButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PauseHUD: SettingsButton is not bound."));
	}

	if (IsValid(MainMenuButton))
	{
		MainMenuButton->OnVacancyButtonClicked.AddUniqueDynamic(this, &UPauseHUD::HandleMainMenuButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PauseHUD: MainMenuButton is not bound."));
	}

	if (IsValid(ExitButton))
	{
		ExitButton->OnVacancyButtonClicked.AddUniqueDynamic(this, &UPauseHUD::HandleExitButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PauseHUD: ExitButton is not bound."));
	}

	bButtonsBound = true;
}

void UPauseHUD::UnbindPauseButtons()
{
	if (!bButtonsBound)
	{
		return;
	}

	if (IsValid(ResumeButton))
	{
		ResumeButton->OnVacancyButtonClicked.RemoveDynamic(this, &UPauseHUD::HandleResumeButtonClicked);
	}

	if (IsValid(SettingsButton))
	{
		SettingsButton->OnVacancyButtonClicked.RemoveDynamic(this, &UPauseHUD::HandleSettingsButtonClicked);
	}

	if (IsValid(MainMenuButton))
	{
		MainMenuButton->OnVacancyButtonClicked.RemoveDynamic(this, &UPauseHUD::HandleMainMenuButtonClicked);
	}

	if (IsValid(ExitButton))
	{
		ExitButton->OnVacancyButtonClicked.RemoveDynamic(this, &UPauseHUD::HandleExitButtonClicked);
	}

	bButtonsBound = false;
}

void UPauseHUD::SetAllPauseButtonsInteractable(const bool bInteractable)
{
	if (IsValid(ResumeButton))
	{
		ResumeButton->SetButtonInteractable(bInteractable);
	}

	if (IsValid(SettingsButton))
	{
		SettingsButton->SetButtonInteractable(bInteractable);
	}

	if (IsValid(MainMenuButton))
	{
		MainMenuButton->SetButtonInteractable(bInteractable);
	}

	if (IsValid(ExitButton))
	{
		ExitButton->SetButtonInteractable(bInteractable);
	}
}

void UPauseHUD::HandleResumeButtonClicked(UVacancyButtonBase* Button)
{
	OnPauseHUDResumeRequested.Broadcast();

	UVacancyMenuUtils::ResumeGame(this, bShowInGameHUDOnResume);
}

void UPauseHUD::HandleSettingsButtonClicked(UVacancyButtonBase* Button)
{
	OnPauseHUDSettingsRequested.Broadcast();

	HandleSettingsRequested();
}

void UPauseHUD::HandleMainMenuButtonClicked(UVacancyButtonBase* Button)
{
	OnPauseHUDMainMenuRequested.Broadcast();

	SetAllPauseButtonsInteractable(false);

	const bool bReturnedToMainMenu =
		UVacancyMenuUtils::ReturnToMainMenu(this, MainMenuLevelName);

	if (!bReturnedToMainMenu)
	{
		SetAllPauseButtonsInteractable(true);
	}
}

void UPauseHUD::HandleExitButtonClicked(UVacancyButtonBase* Button)
{
	OnPauseHUDExitRequested.Broadcast();

	SetAllPauseButtonsInteractable(false);

	UVacancyMenuUtils::QuitGame(this);
}

void UPauseHUD::HandleSettingsRequested_Implementation()
{
	UE_LOG(
		LogTemp,
		Log,
		TEXT("PauseHUD settings requested. Override HandleSettingsRequested in Blueprint or C++ to open your settings menu.")
	);
}