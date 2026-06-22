#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InteractionData.generated.h"

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	None UMETA(DisplayName="None"),
	Inspect UMETA(DisplayName="Inspect"),
	Use UMETA(DisplayName="Use"),
	Open UMETA(DisplayName="Open"),
	Hide UMETA(DisplayName="Hide"),
	Record UMETA(DisplayName="Record"),
	Photograph UMETA(DisplayName="Photograph"),
	Unlock UMETA(DisplayName="Unlock")
};

USTRUCT(BlueprintType)
struct FInteractionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EInteractionType InteractionType = EInteractionType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* InteractionInstigator = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* InteractableActor = nullptr;
};

USTRUCT(BlueprintTYpe)
struct FInteractionVisualInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText InteractionText = FText::FromString("Interact");

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* InteractionIcon = nullptr;
};

USTRUCT(BlueprintType)
struct FInteractionValidationData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer RequiredInteractionTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxInteractionDistance = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText FailureReason = FText::FromString("Invalid Interaction");
};