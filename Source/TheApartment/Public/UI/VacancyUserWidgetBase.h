#pragma once

#include "CoreMinimal.h"
#include "VacancyHUDData.h"
#include "Blueprint/UserWidget.h"
#include "VacancyUserWidgetBase.generated.h"

class AVacancyHUD;

/**
 * Base class for all Vacancy UI widgets.
 *
 * Every HUD screen and HUD element should inherit from this.
 */
UCLASS()
class THEAPARTMENT_API UVacancyUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Vacancy UI")
	void InitializeVacancyWidget(AVacancyHUD* InOwningHUD);

	UFUNCTION(BlueprintCallable, Category = "Vacancy UI")
	void ToggleVacancyWidget(bool bVisible);

	UFUNCTION(BlueprintCallable, Category = "Vacancy UI")
	void RefreshVacancyWidget();

	UFUNCTION(BlueprintPure, Category = "Vacancy UI")
	FVacancyWidgetData GetWidgetData() const { return WidgetData; }

	UFUNCTION(BlueprintPure, Category = "Vacancy UI")
	AVacancyHUD* GetOwningVacancyHUD() const { return OwningVacancyHUD.Get(); }

	UFUNCTION(BlueprintPure, Category = "Vacancy UI")
	bool IsVacancyWidgetInitialized() const { return bVacancyWidgetInitialized; }

protected:

	UFUNCTION(BlueprintNativeEvent, Category = "Vacancy UI")
	void OnVacancyWidgetInitialized();
	virtual void OnVacancyWidgetInitialized_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Vacancy UI")
	void OnToggleVacancyWidget(bool bVisible);
	virtual void OnToggleVacancyWidget_Implementation(bool bVisible);

	UFUNCTION(BlueprintNativeEvent, Category = "Vacancy UI")
	void OnRefreshVacancyWidget();
	virtual void OnRefreshVacancyWidget_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vacancy UI")
	FVacancyWidgetData WidgetData;

private:

	UPROPERTY(Transient)
	TObjectPtr<AVacancyHUD> OwningVacancyHUD = nullptr;

	bool bVacancyWidgetInitialized = false;
};