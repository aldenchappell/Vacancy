#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ToolData.generated.h"

class UBaseToolAbility;

UENUM(BlueprintType)
enum class EToolType: uint8
{
	Flashlight UMETA(DisplayName = "Flashlight"),
	Phone UMETA(DisplayName = "Phone"),
	Camera UMETA(DisplayName = "Camera"),
	Recorder UMETA(DisplayName = "Audio Recorder"),
	MAX
};

USTRUCT(BlueprintType)
struct FToolDataRow
{
	GENERATED_BODY()

	/*
	 * The display name of the tool. This is used in the UI to display the name of the tool.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool Data")
	FName ToolDisplayName = FName("Tool Name");

	/*
	 * The description of the tool. This is used in the UI to display the description of the tool.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool Data")
	FText ToolDescription = FText::FromString("Tool Description");

	/*
	 * The icon of the tool. This is used in the UI to display the icon of the tool.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool Data")
	TSoftObjectPtr<UTexture2D> ToolDisplayIcon = nullptr;

	/*
	 * The type of the tool. This is used to determine the behavior of the tool.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool Data")
	EToolType ToolType = EToolType::MAX;

	/**
	 * Abilities granted when this tool becomes available to the player.
	 *
	 * Most tools have one ability.
	 * The camera has camera-mode and take-photo abilities.
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Tool Data|Abilities")
	TArray<TSubclassOf<UBaseToolAbility>> ToolAbilityClasses = TArray<TSubclassOf<UBaseToolAbility>>();
};
