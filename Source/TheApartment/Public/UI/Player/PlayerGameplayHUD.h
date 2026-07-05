#pragma once

#include "CoreMinimal.h"
#include "UI/VacancyUserWidgetBase.h"
#include "PlayerGameplayHUD.generated.h"

class UCaseClueHUDWidget;
class UPlayerObjectiveHUD;
class UPlayerToolHUDSuite;
class UPlayerCaseInventoryHUD;
class UPlayerActiveToolHUD;

/**
 * Master in-game HUD.
 *
 * This widget owns the gameplay HUD elements that are placed inside the UMG designer.
 */
UCLASS()
class THEAPARTMENT_API UPlayerGameplayHUD : public UVacancyUserWidgetBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UVacancyUserWidgetBase* GetHUDWidgetByElementType(EVacancyHUDElementType HUDElementType) const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UPlayerActiveToolHUD* GetActiveToolHUD() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UPlayerToolHUDSuite* GetToolHUDSuite() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UPlayerCaseInventoryHUD* GetCaseInventoryHUD() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UPlayerObjectiveHUD* GetPlayerObjectiveHUD() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UCaseClueHUDWidget* GetCaseClueHUD() const;

protected:

	virtual void OnVacancyWidgetInitialized_Implementation() override;

private:

	void RegisterHUDWidgetsFromWidgetTree();

	UPROPERTY(Transient)
	TMap<EVacancyHUDElementType, TObjectPtr<UVacancyUserWidgetBase>> RegisteredHUDElements;
};