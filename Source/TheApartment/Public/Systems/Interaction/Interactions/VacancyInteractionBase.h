// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Characters/Player/Interaction/InteractionData.h"
#include "UObject/Object.h"
#include "VacancyInteractionBase.generated.h"

class AVacancyPlayerCharacter;


UCLASS()
class THEAPARTMENT_API UVacancyInteractionBase : public UObject
{
	GENERATED_BODY()

public:

	void InitializeInteraction(const FInteractionInfo& InteractInfo);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Interaction")
	bool CanInteract(AVacancyPlayerCharacter* InteractingCharacter) const;

	UFUNCTION(BlueprintNativeEvent, Category="Interaction")
	void Interact(AVacancyPlayerCharacter* InteractingCharacter);

	FInteractionInfo GetInteractionInfo() const { return InteractionInfo; }
	FInteractionVisualInfo GetVisualInfo() const { return InteractionVisualInfo; }
	FInteractionValidationData GetValidationData() const { return InteractionValidationData; }
protected:

	UPROPERTY(EditAnywhere, Category="Interaction")
	FInteractionInfo InteractionInfo;

	UPROPERTY(EditAnywhere, Category="Interaction")
	FInteractionVisualInfo InteractionVisualInfo;

	UPROPERTY(EditAnywhere, Category="Interaction")
	FInteractionValidationData InteractionValidationData;
	
	UPROPERTY(EditAnywhere, Category="Interaction")
	EInteractionType InteractionType = EInteractionType::Use;

	void OnInteractionSuccessful(const AVacancyPlayerCharacter* InteractingCharacter) const;
	void OnInteractionFailed(const AVacancyPlayerCharacter* InteractingCharacter, const FString& FailureReason) const;
private:

	void SetInteractionOwner(AActor* NewOwner) { OwnerInteractable = NewOwner; }
	TObjectPtr<AActor> OwnerInteractable = nullptr;
};
