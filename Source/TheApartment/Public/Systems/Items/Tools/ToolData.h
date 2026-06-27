#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ToolData.generated.h"

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
	FName ToolDisplayName;

	/*
	 * The description of the tool. This is used in the UI to display the description of the tool.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool Data")
	FText ToolDescription;

	/*
	 * The icon of the tool. This is used in the UI to display the icon of the tool.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool Data")
	UTexture2D* ToolDisplayIcon;

	/*
	 * The type of the tool. This is used to determine the behavior of the tool.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tool Data")
	EToolType ToolType;
};
