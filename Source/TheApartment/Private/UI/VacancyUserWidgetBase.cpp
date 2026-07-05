#include "UI/VacancyUserWidgetBase.h"

#include "UI/VacancyHUD.h"

void UVacancyUserWidgetBase::InitializeVacancyWidget(AVacancyHUD* InOwningHUD)
{
	if (bVacancyWidgetInitialized)
	{
		return;
	}

	if (!IsValid(InOwningHUD))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s failed to initialize: OwningHUD is invalid."), *GetNameSafe(this));
		return;
	}

	OwningVacancyHUD = InOwningHUD;
	bVacancyWidgetInitialized = true;

	OnVacancyWidgetInitialized();

	ToggleVacancyWidget(WidgetData.bAutoShowWidgetOnConstruction);
}

void UVacancyUserWidgetBase::ToggleVacancyWidget(const bool bVisible)
{
	SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	OnToggleVacancyWidget(bVisible);
}

void UVacancyUserWidgetBase::RefreshVacancyWidget()
{
	if (!bVacancyWidgetInitialized)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s RefreshVacancyWidget called before initialization."), *GetNameSafe(this));
		return;
	}

	OnRefreshVacancyWidget();
}

void UVacancyUserWidgetBase::OnVacancyWidgetInitialized_Implementation()
{
}

void UVacancyUserWidgetBase::OnToggleVacancyWidget_Implementation(const bool bVisible)
{
}

void UVacancyUserWidgetBase::OnRefreshVacancyWidget_Implementation()
{
}