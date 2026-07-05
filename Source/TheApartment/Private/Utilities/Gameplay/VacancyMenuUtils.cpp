// Fill out your copyright notice in the Description page of Project Settings.

#include "Utilities/Gameplay/VacancyMenuUtils.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/VacancyHUD.h"

APlayerController* UVacancyMenuUtils::GetPlayerControllerFromContext(const UObject* WorldContextObject)
{
	if (!IsValid(WorldContextObject))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetPlayerControllerFromContext failed: WorldContextObject is invalid."));
		return nullptr;
	}

	if (const UUserWidget* UserWidget = Cast<UUserWidget>(WorldContextObject))
	{
		if (APlayerController* OwningPlayer = UserWidget->GetOwningPlayer())
		{
			return OwningPlayer;
		}
	}

	if (const APawn* Pawn = Cast<APawn>(WorldContextObject))
	{
		return Cast<APlayerController>(Pawn->GetController());
	}

	if (const AHUD* HUD = Cast<AHUD>(WorldContextObject))
	{
		return HUD->GetOwningPlayerController();
	}

	return UGameplayStatics::GetPlayerController(WorldContextObject, 0);
}

AVacancyHUD* UVacancyMenuUtils::GetVacancyHUDFromContext(const UObject* WorldContextObject)
{
	APlayerController* PlayerController = GetPlayerControllerFromContext(WorldContextObject);
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetVacancyHUDFromContext failed: PlayerController is invalid."));
		return nullptr;
	}

	AVacancyHUD* VacancyHUD = Cast<AVacancyHUD>(PlayerController->GetHUD());
	if (!IsValid(VacancyHUD))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetVacancyHUDFromContext failed: HUD is not AVacancyHUD."));
		return nullptr;
	}

	return VacancyHUD;
}

bool UVacancyMenuUtils::SetGamePaused(const UObject* WorldContextObject, const bool bPaused)
{
	if (!IsValid(WorldContextObject))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetGamePaused failed: WorldContextObject is invalid."));
		return false;
	}

	return UGameplayStatics::SetGamePaused(WorldContextObject, bPaused);
}

bool UVacancyMenuUtils::ApplyGameOnlyInputMode(const UObject* WorldContextObject)
{
	APlayerController* PlayerController = GetPlayerControllerFromContext(WorldContextObject);
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyGameOnlyInputMode failed: PlayerController is invalid."));
		return false;
	}

	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);

	PlayerController->bShowMouseCursor = false;
	PlayerController->bEnableClickEvents = false;
	PlayerController->bEnableMouseOverEvents = false;

	return true;
}

bool UVacancyMenuUtils::ApplyUIOnlyInputMode(const UObject* WorldContextObject, UUserWidget* WidgetToFocus)
{
	APlayerController* PlayerController = GetPlayerControllerFromContext(WorldContextObject);
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyUIOnlyInputMode failed: PlayerController is invalid."));
		return false;
	}

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	if (IsValid(WidgetToFocus))
	{
		InputMode.SetWidgetToFocus(WidgetToFocus->TakeWidget());
	}

	PlayerController->SetInputMode(InputMode);

	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;

	return true;
}

bool UVacancyMenuUtils::ApplyGameAndUIInputMode(const UObject* WorldContextObject, UUserWidget* WidgetToFocus)
{
	APlayerController* PlayerController = GetPlayerControllerFromContext(WorldContextObject);
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyGameAndUIInputMode failed: PlayerController is invalid."));
		return false;
	}

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);

	if (IsValid(WidgetToFocus))
	{
		InputMode.SetWidgetToFocus(WidgetToFocus->TakeWidget());
	}

	PlayerController->SetInputMode(InputMode);

	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;

	return true;
}

bool UVacancyMenuUtils::ShowHUDScreen(const UObject* WorldContextObject, const EVacancyHUDType HUDType)
{
	if (HUDType == EVacancyHUDType::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShowHUDScreen failed: HUDType is None."));
		return false;
	}

	AVacancyHUD* VacancyHUD = GetVacancyHUDFromContext(WorldContextObject);
	if (!IsValid(VacancyHUD))
	{
		return false;
	}

	VacancyHUD->ShowOnlyHUDScreen(HUDType);
	return true;
}

bool UVacancyMenuUtils::SetHUDScreenVisible(
	const UObject* WorldContextObject,
	const EVacancyHUDType HUDType,
	const bool bVisible)
{
	if (HUDType == EVacancyHUDType::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetHUDScreenVisible failed: HUDType is None."));
		return false;
	}

	AVacancyHUD* VacancyHUD = GetVacancyHUDFromContext(WorldContextObject);
	if (!IsValid(VacancyHUD))
	{
		return false;
	}

	VacancyHUD->SetHUDScreenVisible(HUDType, bVisible);
	return true;
}

bool UVacancyMenuUtils::ResumeGame(const UObject* WorldContextObject, const bool bShowInGameHUD)
{
	SetGamePaused(WorldContextObject, false);
	ApplyGameOnlyInputMode(WorldContextObject);

	if (bShowInGameHUD)
	{
		ShowHUDScreen(WorldContextObject, EVacancyHUDType::InGame);
	}
	else
	{
		SetHUDScreenVisible(WorldContextObject, EVacancyHUDType::Paused, false);
	}

	return true;
}

bool UVacancyMenuUtils::OpenPauseMenu(const UObject* WorldContextObject, UUserWidget* PauseWidgetToFocus)
{
	SetGamePaused(WorldContextObject, true);
	ShowHUDScreen(WorldContextObject, EVacancyHUDType::Paused);
	ApplyUIOnlyInputMode(WorldContextObject, PauseWidgetToFocus);

	return true;
}

bool UVacancyMenuUtils::ReturnToMainMenu(const UObject* WorldContextObject, const FName MainMenuLevelName)
{
	if (MainMenuLevelName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("ReturnToMainMenu failed: MainMenuLevelName is None."));
		return false;
	}

	SetGamePaused(WorldContextObject, false);
	ApplyGameOnlyInputMode(WorldContextObject);

	UGameplayStatics::OpenLevel(WorldContextObject, MainMenuLevelName);
	return true;
}

bool UVacancyMenuUtils::QuitGame(const UObject* WorldContextObject)
{
	APlayerController* PlayerController = GetPlayerControllerFromContext(WorldContextObject);

	UKismetSystemLibrary::QuitGame(
		WorldContextObject,
		PlayerController,
		EQuitPreference::Quit,
		false
	);

	return true;
}