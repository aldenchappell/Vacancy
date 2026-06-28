#pragma once

#include "CoreMinimal.h"
#include "VacancyHUDData.generated.h"

UENUM(BlueprintType)
enum class EVacancyHUDType : uint8
{
	None,
	ToolHUD UMETA(DisplayName = "Tool HUD"),
	ToolHUDSuite UMETA(DisplayName = "Tool HUD Suite"),
	CaseInventory UMETA(DisplayName = "Case Inventory"),
};

USTRUCT(BlueprintType)
struct FVacancyWidgetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD Data")
	EVacancyHUDType HUDType = EVacancyHUDType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD Data")
	bool bAutoShowWidgetOnConstruction = true; 
};
