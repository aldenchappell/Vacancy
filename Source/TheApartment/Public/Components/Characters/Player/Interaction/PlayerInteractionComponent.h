// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionData.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractionComponent.generated.h"

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

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEAPARTMENT_API UPlayerInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerInteractionComponent();

	UFUNCTION()
	bool TryInteractWith(AActor* InInteractableActor);
	
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	
	void StartInteraction(const FInteractionInfo& InteractionInfo) const;
	void EndInteraction(const FInteractionInfo& InteractionInfo) const;

	UPROPERTY(EditAnywhere, Category="Interaction")
	FInteractionScanInfo InteractionScanInfo;
	
private:

	void ScanForInteractables();
	TObjectPtr<AActor> LastActiveInteractable = nullptr;

	float TimeSinceLastScan = 0.f;

	static AActor* ActorToInteractable(AActor* InActor);

	TObjectPtr<AVacancyPlayerCharacter> PlayerCharacter = nullptr;
};
