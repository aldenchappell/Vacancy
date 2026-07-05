#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/VacancyHUDData.h"
#include "VacancyHUD.generated.h"

class UCaseClueHUDWidget;
class UPlayerGameplayHUD;
class UPlayerToolHUDSuite;
class UPlayerCaseInventoryHUD;
class UPlayerActiveToolHUD;
class UPlayerObjectiveHUD;
class UVacancyUserWidgetBase;

UCLASS()
class THEAPARTMENT_API AVacancyHUD : public AHUD
{
	GENERATED_BODY()

public:

	AVacancyHUD();

	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UVacancyUserWidgetBase* GetHUDScreenByType(EVacancyHUDType HUDType) const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UVacancyUserWidgetBase* GetHUDElementByElementType(EVacancyHUDElementType HUDElementType) const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetHUDScreenVisible(EVacancyHUDType HUDType, bool bVisible);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowOnlyHUDScreen(EVacancyHUDType HUDType);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetHUDElementVisible(EVacancyHUDElementType HUDElementType, bool bVisible);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void RefreshHUDElement(EVacancyHUDElementType HUDElementType);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void RefreshHUDScreen(EVacancyHUDType HUDType);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UPlayerGameplayHUD* GetGameplayHUD() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UCaseClueHUDWidget* GetCaseDisplayHUD() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UPlayerActiveToolHUD* GetActiveToolHUD() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UPlayerCaseInventoryHUD* GetCaseInventoryHUD() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UPlayerToolHUDSuite* GetToolHUDSuite() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UPlayerObjectiveHUD* GetPlayerObjectiveHUD() const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TArray<FVacancyHUDScreenConfig> HUDScreensToCreate;

private:

	void CreateConfiguredHUDScreens();

	UVacancyUserWidgetBase* CreateHUDScreen(const FVacancyHUDScreenConfig& ScreenConfig);

	UPROPERTY(Transient)
	TMap<EVacancyHUDType, TObjectPtr<UVacancyUserWidgetBase>> HUDScreens;
};