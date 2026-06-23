// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Characters/Player/Interaction/InteractionData.h"
#include "UObject/Object.h"
#include "VacancyInteractionBase.generated.h"

class AVacancyPlayerCharacter;


UCLASS(Blueprintable)
class THEAPARTMENT_API UVacancyInteractionBase : public UObject
{
	GENERATED_BODY()

public:

	void InitializeInteraction(const FInteractionInfo& InteractInfo);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Interaction")
	bool CanInteract(AVacancyPlayerCharacter* InteractingCharacter) const;

	UFUNCTION(BlueprintNativeEvent, Category="Interaction")
	void Interact(AVacancyPlayerCharacter* InteractingCharacter);

	FInteractionInfo GetInteractionInfo() const { return InteractionData; }
	FInteractionVisualInfo GetVisualInfo() const { return InteractionData.InteractionVisualInfo; }
	FInteractionValidationData GetValidationData() const { return InteractionData.InteractionValidationData; }
protected:

	UPROPERTY(EditAnywhere, Category="Interaction")
	FInteractionInfo InteractionData;

	void OnInteractionSuccessful(const AVacancyPlayerCharacter* InteractingCharacter) const;
	void OnInteractionFailed(const AVacancyPlayerCharacter* InteractingCharacter, const FString& FailureReason) const;
private:

	void SetInteractionOwner(AActor* NewOwner) { OwnerInteractable = NewOwner; }
	TObjectPtr<AActor> OwnerInteractable = nullptr;
};
