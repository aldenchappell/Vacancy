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

	/*
	 * A tag that can be used to identify the interaction. This can be used to filter interactions in the player interaction component.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName InteractionIdentifier = NAME_None;

	/*
	 * The type of interaction. This can be used to filter interactions in the player interaction component.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EInteractionType InteractionType = EInteractionType::None;

	/*
	 * The actor that is instigating the interaction. This is usually the player character.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* InteractionInstigator = nullptr;

	/*
	 * The actor that is being interacted with. This is usually the interactable actor.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* InteractableActor = nullptr;
};

USTRUCT(BlueprintType)
struct FInteractionVisualInfo
{
	GENERATED_BODY()

	/*
	 * A gameplay tag that can be used to identify the interaction. This can be used to filter interactions in the player interaction component.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag InteractionTag;
	
	/*
	 * The icon that will be displayed to the player when they are looking at the interactable actor.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* InteractionIcon = nullptr;
};

USTRUCT(BlueprintType)
struct FInteractionValidationData
{
	GENERATED_BODY()

	/*
	 * If the interacting character does not have all of these tags, the interaction will fail.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer RequiredInteractionTags;

	/*
	 * The max distance in which the player can be from the interactable actor to successfully interact with it.
	 * If the player is further away than this distance, the interaction will fail.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxInteractionDistance = 400.f;

	/*
	 * The reason why the interaction failed. This can be used to display a message to the player.
	 */
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