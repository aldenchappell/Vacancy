// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/VacancyUserWidgetBase.h"
#include "PauseHUD.generated.h"

class UVacancyButtonBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPauseHUDEvent);

UCLASS()
class THEAPARTMENT_API UPauseHUD : public UVacancyUserWidgetBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Pause HUD")
	FOnPauseHUDEvent OnPauseHUDResumeRequested;

	UPROPERTY(BlueprintAssignable, Category = "Pause HUD")
	FOnPauseHUDEvent OnPauseHUDSettingsRequested;

	UPROPERTY(BlueprintAssignable, Category = "Pause HUD")
	FOnPauseHUDEvent OnPauseHUDMainMenuRequested;

	UPROPERTY(BlueprintAssignable, Category = "Pause HUD")
	FOnPauseHUDEvent OnPauseHUDExitRequested;

protected:

	virtual void NativeDestruct() override;

	virtual void OnVacancyWidgetInitialized_Implementation() override;
	virtual void OnToggleVacancyWidget_Implementation(bool bVisible) override;
	virtual void OnRefreshVacancyWidget_Implementation() override;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UVacancyButtonBase> ResumeButton = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UVacancyButtonBase> SettingsButton = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UVacancyButtonBase> MainMenuButton = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UVacancyButtonBase> ExitButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pause HUD")
	FName MainMenuLevelName = TEXT("MainMenu");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pause HUD")
	bool bPauseGameWhenShown = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pause HUD")
	bool bFocusResumeButtonWhenShown = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pause HUD")
	bool bShowInGameHUDOnResume = true;

	UFUNCTION(BlueprintNativeEvent, Category = "Pause HUD")
	void HandleSettingsRequested();
	virtual void HandleSettingsRequested_Implementation();

private:

	void BindPauseButtons();
	void UnbindPauseButtons();

	void ConfigurePauseButtons();
	void SetAllPauseButtonsInteractable(bool bInteractable);

	UFUNCTION()
	void HandleResumeButtonClicked(UVacancyButtonBase* Button);

	UFUNCTION()
	void HandleSettingsButtonClicked(UVacancyButtonBase* Button);

	UFUNCTION()
	void HandleMainMenuButtonClicked(UVacancyButtonBase* Button);

	UFUNCTION()
	void HandleExitButtonClicked(UVacancyButtonBase* Button);

	bool bButtonsBound = false;
};