// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/Gameplay/VacancyUIUtils.h"

#include "Blueprint/UserWidget.h"
#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "UI/VacancyHUD.h"
#include "UI/VacancyUserWidgetBase.h"
#include "UI/Inventory/Evidence/PlayerCaseInventoryHUD.h"
#include "UI/Inventory/Tools/PlayerToolHUDSuite.h"
#include "UI/Tools/PlayerActiveToolHUD.h"
#include "Utilities/Subsystems/Gameplay/VacancyFadeSubsystem.h"

AVacancyHUD* UVacancyUIUtils::GetVacancyHUD(const AVacancyPlayerCharacter* PlayerCharacter)
{
	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetVacancyHUD called with null PlayerCharacter."));
		return nullptr;
	}

	const APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController());
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetVacancyHUD failed: PlayerController is null."));
		return nullptr;
	}

	return Cast<AVacancyHUD>(PlayerController->GetHUD());
}

void UVacancyUIUtils::ShowWidget(UUserWidget* Widget, const bool bShow)
{
	if (!IsValid(Widget))
	{
		UE_LOG(LogTemp, Warning, TEXT("ShowWidget called with null Widget."));
		return;
	}

	Widget->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UVacancyUIUtils::SetWidgetVisibility(UUserWidget* Widget, const ESlateVisibility Visibility)
{
	if (!IsValid(Widget))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetWidgetVisibility called with null Widget."));
		return;
	}

	Widget->SetVisibility(Visibility);
}

void UVacancyUIUtils::SetWidgetText(UTextBlock* TextBlock, const FText& NewText)
{
	if (!IsValid(TextBlock))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetWidgetTextColor called with null TextBlock."));
		return;
	}

	if (NewText.EqualTo(TextBlock->GetText()))
	{
		return; // No change needed, already has the desired text
	}
	
	TextBlock->SetText(NewText);
}

void UVacancyUIUtils::SetWidgetTextColor(UTextBlock* TextBlock, const FSlateColor& NewColor)
{
	if (!IsValid(TextBlock))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetWidgetTextColor called with null TextBlock."));
		return;
	}

	if (NewColor == TextBlock->GetColorAndOpacity())
	{
		return; // No change needed, already in the desired color
	}

	TextBlock->SetColorAndOpacity(NewColor);
}

void UVacancyUIUtils::FadeInWidget(UUserWidget* Widget, float Duration)
{
	if (!IsValid(Widget))
	{
		UE_LOG(LogTemp, Warning, TEXT("FadeInWidget called with null Widget."));
		return;
	}

	if (Duration <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("FadeInWidget called with non-positive Duration."));
		return;
	}

	UVacancyFadeSubsystem* FadeSubsystem = UGameplayStatics::GetGameInstance(Widget)->GetSubsystem<UVacancyFadeSubsystem>();
	if (!IsValid(FadeSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("FadeInWidget failed: FadeSubsystem is null."));
		return;
	}

	//FadeSubsystem->StartFade()
}

void UVacancyUIUtils::FadeOutWidget(UUserWidget* Widget, float Duration)
{
	if (!IsValid(Widget))
	{
		UE_LOG(LogTemp, Warning, TEXT("FadeOutWidget called with null Widget."));
		return;
	}

	if (Duration <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("FadeOutWidget called with non-positive Duration."));
		return;
	}

	UVacancyFadeSubsystem* FadeSubsystem = UGameplayStatics::GetGameInstance(Widget)->GetSubsystem<UVacancyFadeSubsystem>();
	if (!IsValid(FadeSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("FadeOutWidget failed: FadeSubsystem is null."));
		return;
	}

	if (FadeSubsystem->IsFading())
	{
		FadeSubsystem->InterruptFade();
		//FadeSubsystem->StartFade()
	}
}

UVacancyUserWidgetBase* UVacancyUIUtils::GetHUDElementByType(const AVacancyPlayerCharacter* PlayerCharacter,
	const EVacancyHUDType HUDType)
{
	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetHUDElementByType called with null PlayerCharacter."));
		return nullptr;
	}

	if (HUDType == EVacancyHUDType::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetHUDElementByType called with HUDType::None."));
		return nullptr;
	}

	const AVacancyHUD* VacancyHUD = GetVacancyHUD(PlayerCharacter);
	if (!IsValid(VacancyHUD))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetHUDElementByType failed: VacancyHUD is null."));
		return nullptr;
	}
	
	switch (HUDType)
	{
		case EVacancyHUDType::ToolHUD:
		{
			UPlayerActiveToolHUD* ToolHUD = VacancyHUD->GetActiveToolHUD();
			if (!IsValid(ToolHUD))
			{
				UE_LOG(LogTemp, Warning, TEXT("GetHUDElementByType failed: HUDWidget is not valid."));
				return nullptr;
			}
			return ToolHUD;
		}
		case EVacancyHUDType::ToolHUDSuite:
		{
			UPlayerToolHUDSuite* ToolHUDSuite = VacancyHUD->GetToolHUDSuite();
			if (!IsValid(ToolHUDSuite))
			{
				UE_LOG(LogTemp, Warning, TEXT("GetHUDElementByType failed: ToolHUDSuite is not valid."));
				return nullptr;
			}
			return ToolHUDSuite;
		}
		case EVacancyHUDType::CaseInventory:
		{
			UPlayerCaseInventoryHUD* CaseInventoryHUD = VacancyHUD->GetCaseInventoryHUD();
			if (!IsValid(CaseInventoryHUD))
			{
				UE_LOG(LogTemp, Warning, TEXT("GetHUDElementByType failed: CaseInventoryHUD is not valid."));
				return nullptr;
			}

			return CaseInventoryHUD;
		}
		default:
			UE_LOG(LogTemp, Warning, TEXT("GetHUDElementByType failed: Unhandled HUDType."));
			return nullptr;
		break;
	}

	return nullptr; // Default return if no valid HUD element is found
}

