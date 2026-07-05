#include "UI/Player/PlayerGameplayHUD.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Widget.h"
#include "UI/Case/CaseClueHUDWidget.h"
#include "UI/Inventory/Evidence/PlayerCaseInventoryHUD.h"
#include "UI/Inventory/Tools/PlayerToolHUDSuite.h"
#include "UI/Objectives/PlayerObjectiveHUD.h"
#include "UI/Tools/PlayerActiveToolHUD.h"

void UPlayerGameplayHUD::OnVacancyWidgetInitialized_Implementation()
{
	Super::OnVacancyWidgetInitialized_Implementation();

	RegisterHUDWidgetsFromWidgetTree();
}

void UPlayerGameplayHUD::RegisterHUDWidgetsFromWidgetTree()
{
	RegisteredHUDElements.Empty();

	if (!WidgetTree)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s failed to register HUD widgets: WidgetTree is invalid."), *GetNameSafe(this));
		return;
	}

	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
		UVacancyUserWidgetBase* VacancyWidget = Cast<UVacancyUserWidgetBase>(Widget);
		if (!IsValid(VacancyWidget))
		{
			return;
		}

		const FVacancyWidgetData WidgetData = VacancyWidget->GetWidgetData();

		if (WidgetData.HUDElementType == EVacancyHUDElementType::None)
		{
			return;
		}

		if (RegisteredHUDElements.Contains(WidgetData.HUDElementType))
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("%s is overwriting duplicate HUD element type %d with widget %s."),
				*GetNameSafe(this),
				static_cast<int32>(WidgetData.HUDElementType),
				*GetNameSafe(VacancyWidget)
			);
		}

		RegisteredHUDElements.Add(WidgetData.HUDElementType, VacancyWidget);

		VacancyWidget->InitializeVacancyWidget(GetOwningVacancyHUD());
	});
}

UVacancyUserWidgetBase* UPlayerGameplayHUD::GetHUDWidgetByElementType(
	const EVacancyHUDElementType HUDElementType) const
{
	if (HUDElementType == EVacancyHUDElementType::None)
	{
		return nullptr;
	}

	if (const TObjectPtr<UVacancyUserWidgetBase>* FoundWidget = RegisteredHUDElements.Find(HUDElementType))
	{
		return FoundWidget->Get();
	}

	return nullptr;
}

UPlayerActiveToolHUD* UPlayerGameplayHUD::GetActiveToolHUD() const
{
	return Cast<UPlayerActiveToolHUD>(GetHUDWidgetByElementType(EVacancyHUDElementType::ToolHUD));
}

UPlayerToolHUDSuite* UPlayerGameplayHUD::GetToolHUDSuite() const
{
	return Cast<UPlayerToolHUDSuite>(GetHUDWidgetByElementType(EVacancyHUDElementType::ToolHUDSuite));
}

UPlayerCaseInventoryHUD* UPlayerGameplayHUD::GetCaseInventoryHUD() const
{
	return Cast<UPlayerCaseInventoryHUD>(GetHUDWidgetByElementType(EVacancyHUDElementType::CaseInventory));
}

UPlayerObjectiveHUD* UPlayerGameplayHUD::GetPlayerObjectiveHUD() const
{
	return Cast<UPlayerObjectiveHUD>(GetHUDWidgetByElementType(EVacancyHUDElementType::ObjectiveHUD));
}

UCaseClueHUDWidget* UPlayerGameplayHUD::GetCaseClueHUD() const
{
	return Cast<UCaseClueHUDWidget>(GetHUDWidgetByElementType(EVacancyHUDElementType::CaseHUD));
}