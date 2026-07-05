#include "UI/VacancyHUD.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "UI/Case/CaseClueHUDWidget.h"
#include "UI/Inventory/Evidence/PlayerCaseInventoryHUD.h"
#include "UI/Inventory/Tools/PlayerToolHUDSuite.h"
#include "UI/Objectives/PlayerObjectiveHUD.h"
#include "UI/Player/PlayerGameplayHUD.h"
#include "UI/Tools/PlayerActiveToolHUD.h"
#include "UI/VacancyUserWidgetBase.h"

AVacancyHUD::AVacancyHUD()
{
}

void AVacancyHUD::BeginPlay()
{
	Super::BeginPlay();

	CreateConfiguredHUDScreens();
}

void AVacancyHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AVacancyHUD::CreateConfiguredHUDScreens()
{
	if (HUDScreensToCreate.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AVacancyHUD has no HUD screens configured."));
		return;
	}

	for (const FVacancyHUDScreenConfig& ScreenConfig : HUDScreensToCreate)
	{
		if (!ScreenConfig.bCreateOnBeginPlay)
		{
			continue;
		}

		CreateHUDScreen(ScreenConfig);
	}
}

UVacancyUserWidgetBase* AVacancyHUD::CreateHUDScreen(const FVacancyHUDScreenConfig& ScreenConfig)
{
	if (ScreenConfig.HUDType == EVacancyHUDType::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateHUDScreen failed: HUDType is None."));
		return nullptr;
	}

	if (!IsValid(ScreenConfig.WidgetClass))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("CreateHUDScreen failed: WidgetClass is invalid for HUDType %d."),
			static_cast<int32>(ScreenConfig.HUDType)
		);

		return nullptr;
	}

	if (HUDScreens.Contains(ScreenConfig.HUDType))
	{
		return HUDScreens[ScreenConfig.HUDType];
	}

	APlayerController* OwningPC = GetOwningPlayerController();
	if (!IsValid(OwningPC))
	{
		OwningPC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	}

	if (!IsValid(OwningPC))
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateHUDScreen failed: no valid PlayerController."));
		return nullptr;
	}

	UVacancyUserWidgetBase* NewHUDScreen =
		CreateWidget<UVacancyUserWidgetBase>(OwningPC, ScreenConfig.WidgetClass);

	if (!IsValid(NewHUDScreen))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("CreateHUDScreen failed to create widget class %s."),
			*GetNameSafe(ScreenConfig.WidgetClass)
		);

		return nullptr;
	}

	NewHUDScreen->AddToViewport(ScreenConfig.ZOrder);
	NewHUDScreen->InitializeVacancyWidget(this);
	NewHUDScreen->ToggleVacancyWidget(ScreenConfig.bShowOnBeginPlay);

	HUDScreens.Add(ScreenConfig.HUDType, NewHUDScreen);

	return NewHUDScreen;
}

UVacancyUserWidgetBase* AVacancyHUD::GetHUDScreenByType(const EVacancyHUDType HUDType) const
{
	if (HUDType == EVacancyHUDType::None)
	{
		return nullptr;
	}

	if (const TObjectPtr<UVacancyUserWidgetBase>* FoundWidget = HUDScreens.Find(HUDType))
	{
		return FoundWidget->Get();
	}

	return nullptr;
}

UVacancyUserWidgetBase* AVacancyHUD::GetHUDElementByElementType(
	const EVacancyHUDElementType HUDElementType) const
{
	if (HUDElementType == EVacancyHUDElementType::None)
	{
		return nullptr;
	}

	for (const TPair<EVacancyHUDType, TObjectPtr<UVacancyUserWidgetBase>>& HUDPair : HUDScreens)
	{
		UVacancyUserWidgetBase* HUDScreen = HUDPair.Value.Get();
		if (!IsValid(HUDScreen))
		{
			continue;
		}

		if (HUDScreen->GetWidgetData().HUDElementType == HUDElementType)
		{
			return HUDScreen;
		}

		if (const UPlayerGameplayHUD* GameplayHUD = Cast<UPlayerGameplayHUD>(HUDScreen))
		{
			if (UVacancyUserWidgetBase* FoundElement = GameplayHUD->GetHUDWidgetByElementType(HUDElementType))
			{
				return FoundElement;
			}
		}
	}

	return nullptr;
}

void AVacancyHUD::SetHUDScreenVisible(const EVacancyHUDType HUDType, const bool bVisible)
{
	UVacancyUserWidgetBase* HUDScreen = GetHUDScreenByType(HUDType);
	if (!IsValid(HUDScreen))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("SetHUDScreenVisible failed: no HUD screen found for HUDType %d."),
			static_cast<int32>(HUDType)
		);

		return;
	}

	HUDScreen->ToggleVacancyWidget(bVisible);
}

void AVacancyHUD::ShowOnlyHUDScreen(const EVacancyHUDType HUDType)
{
	for (const TPair<EVacancyHUDType, TObjectPtr<UVacancyUserWidgetBase>>& HUDPair : HUDScreens)
	{
		if (UVacancyUserWidgetBase* HUDScreen = HUDPair.Value.Get())
		{
			HUDScreen->ToggleVacancyWidget(HUDPair.Key == HUDType);
		}
	}
}

void AVacancyHUD::SetHUDElementVisible(const EVacancyHUDElementType HUDElementType, const bool bVisible)
{
	UVacancyUserWidgetBase* HUDElement = GetHUDElementByElementType(HUDElementType);
	if (!IsValid(HUDElement))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("SetHUDElementVisible failed: no HUD element found for HUDElementType %d."),
			static_cast<int32>(HUDElementType)
		);

		return;
	}

	HUDElement->ToggleVacancyWidget(bVisible);
}

void AVacancyHUD::RefreshHUDElement(const EVacancyHUDElementType HUDElementType)
{
	UVacancyUserWidgetBase* HUDElement = GetHUDElementByElementType(HUDElementType);
	if (!IsValid(HUDElement))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("RefreshHUDElement failed: no HUD element found for HUDElementType %d."),
			static_cast<int32>(HUDElementType)
		);

		return;
	}

	HUDElement->RefreshVacancyWidget();
}

void AVacancyHUD::RefreshHUDScreen(const EVacancyHUDType HUDType)
{
	UVacancyUserWidgetBase* HUDScreen = GetHUDScreenByType(HUDType);
	if (!IsValid(HUDScreen))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("RefreshHUDScreen failed: no HUD screen found for HUDType %d."),
			static_cast<int32>(HUDType)
		);

		return;
	}

	HUDScreen->RefreshVacancyWidget();
}

UPlayerGameplayHUD* AVacancyHUD::GetGameplayHUD() const
{
	return Cast<UPlayerGameplayHUD>(GetHUDScreenByType(EVacancyHUDType::InGame));
}

UCaseClueHUDWidget* AVacancyHUD::GetCaseDisplayHUD() const
{
	return Cast<UCaseClueHUDWidget>(GetHUDElementByElementType(EVacancyHUDElementType::CaseHUD));
}

UPlayerActiveToolHUD* AVacancyHUD::GetActiveToolHUD() const
{
	return Cast<UPlayerActiveToolHUD>(GetHUDElementByElementType(EVacancyHUDElementType::ToolHUD));
}

UPlayerCaseInventoryHUD* AVacancyHUD::GetCaseInventoryHUD() const
{
	return Cast<UPlayerCaseInventoryHUD>(GetHUDElementByElementType(EVacancyHUDElementType::CaseInventory));
}

UPlayerToolHUDSuite* AVacancyHUD::GetToolHUDSuite() const
{
	return Cast<UPlayerToolHUDSuite>(GetHUDElementByElementType(EVacancyHUDElementType::ToolHUDSuite));
}

UPlayerObjectiveHUD* AVacancyHUD::GetPlayerObjectiveHUD() const
{
	return Cast<UPlayerObjectiveHUD>(GetHUDElementByElementType(EVacancyHUDElementType::ObjectiveHUD));
}