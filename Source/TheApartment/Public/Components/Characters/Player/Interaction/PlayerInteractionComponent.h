// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "InteractionData.h"
#include "Components/ActorComponent.h"
#include "Systems/Interaction/Interactions/VacancyInteractionBase.h"
#include "PlayerInteractionComponent.generated.h"

class UWidgetComponent;
class UInteractionWidget;
class AVacancyPlayerCharacter;

USTRUCT(BlueprintType)
struct FInteractionScanInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float TimeBetweenScans = .2f;

	UPROPERTY(EditAnywhere)
	float MaxInteractionScanDistance = 600.f;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;
};

USTRUCT(BlueprintType)
struct FDefaultInteraction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FInteractionInfo> DefaultInteractionInfos;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<UVacancyInteractionBase>> DefaultInteractions;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEAPARTMENT_API UPlayerInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerInteractionComponent();

	UFUNCTION()
	bool TryInteractWithActiveInteractable() const;

	//Default Interaction Management
	bool TryUseInteraction(const FInteractionInfo& InteractionInfo) const;
	
	void EnterHide() const;
	void ExitHide() const;

	bool CanExecuteInteractions() const;

	void ApplyActiveInteractionTags(const FGameplayTagContainer& TagsToApply);
	void RemoveActiveInteractionTags(const FGameplayTagContainer& TagsToRemove);
	
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	
	void StartInteraction(const FInteractionInfo& InteractionInfo) const;
	void EndInteraction(const FInteractionInfo& InteractionInfo) const;

	UPROPERTY(EditAnywhere, Category="Interaction")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;
	
	UPROPERTY(EditAnywhere, Category="Interaction")
	FInteractionScanInfo InteractionScanInfo;

	UPROPERTY(EditInstanceOnly, Category="Interaction")
	TArray<FDefaultInteraction> DefaultInteractions;

	UPROPERTY(EditAnywhere, Category="Interaction|Tags")
	FGameplayTagContainer DefaultInteractionTags;

	UPROPERTY(EditAnywhere, Category="Interaction|Tags")
	FGameplayTagContainer InteractionBlockingTags;
private:

	FGameplayTagContainer ActiveInteractionTags;

	void ScanForInteractables();
	TObjectPtr<AActor> LastActiveInteractable = nullptr;
	TObjectPtr<AVacancyPlayerCharacter> PlayerCharacter = nullptr;
	float TimeSinceLastScan = 0.f;

	void InitializeDefaultInteractions(const TArray<FName>& IgnoredInteractionIdentifiers = TArray<FName>());
	
	//Helpers
	static AActor* ActorToInteractable(AActor* InActor);

	void CreateInteractionWidgetComp();
	void ToggleInteractionWidgetCompVis(const bool bVisible) const;
	void UpdateInteractionWidgetLoc(const AActor* InteractableActor) const;
	UPROPERTY()
	UInteractionWidget* InteractionWidgetInstance = nullptr;
	UPROPERTY()
	UWidgetComponent* InteractionWidgetCompInstance = nullptr;
};
