#pragma once

#include "CoreMinimal.h"
#include "VacancyHUDData.generated.h"

class UVacancyUserWidgetBase;

UENUM(BlueprintType)
enum class EVacancyHUDType : uint8
{
	None,
	InGame UMETA(DisplayName = "In Game"),
	Paused UMETA(DisplayName = "Paused"),
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EVacancyHUDElementType : uint8
{
	None,
	CaseHUD UMETA(DisplayName = "Case HUD"),
	ToolHUD UMETA(DisplayName = "Tool HUD"),
	ToolHUDSuite UMETA(DisplayName = "Tool HUD Suite"),
	CaseInventory UMETA(DisplayName = "Case Inventory"),
	ObjectiveHUD UMETA(DisplayName = "Objective HUD"),
	InteractionPrompt UMETA(DisplayName = "Interaction Prompt"),
	MAX UMETA(Hidden)
};

/**
 * Data that every Vacancy widget carries.
 *
 * Top-level screen example:
 * HUDType = InGame
 * HUDElementType = None
 *
 * Child element example:
 * HUDType = InGame
 * HUDElementType = ToolHUD
 */
USTRUCT(BlueprintType)
struct FVacancyWidgetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD Data")
	EVacancyHUDType HUDType = EVacancyHUDType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD Data")
	EVacancyHUDElementType HUDElementType = EVacancyHUDElementType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD Data")
	bool bAutoShowWidgetOnConstruction = false;
};

/**
 * Config used by AVacancyHUD to create top-level HUD screens.
 */
USTRUCT(BlueprintType)
struct FVacancyHUDScreenConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD Screen")
	EVacancyHUDType HUDType = EVacancyHUDType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD Screen")
	TSubclassOf<UVacancyUserWidgetBase> WidgetClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD Screen")
	int32 ZOrder = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD Screen")
	bool bCreateOnBeginPlay = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD Screen")
	bool bShowOnBeginPlay = false;
};