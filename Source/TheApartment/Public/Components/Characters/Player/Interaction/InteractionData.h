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
	Close UMETA(DisplayName="Close"),
	Hide UMETA(DisplayName="Hide"),
	Record UMETA(DisplayName="Record"),
	Photograph UMETA(DisplayName="Photograph"),
	Unlock UMETA(DisplayName="Unlock")
};

USTRUCT(BlueprintType)
struct FInteractionBasicInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName InteractionIdentifier = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EInteractionType InteractionType = EInteractionType::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* InteractionInstigator = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* InteractableActor = nullptr;
};

USTRUCT(BlueprintTYpe)
struct FInteractionVisualInfo
{
	GENERATED_BODY()

	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// FText InteractionText = FText::FromString("Interact");

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

USTRUCT(BlueprintType)
struct FInteractionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FInteractionBasicInfo InteractionBasicInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FInteractionVisualInfo InteractionVisualInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FInteractionValidationData InteractionValidationData;
};