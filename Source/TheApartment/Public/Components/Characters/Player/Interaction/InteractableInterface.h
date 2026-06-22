// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionData.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

class UVacancyInteractionBase;
class AVacancyPlayerCharacter;
// This class does not need to be modified.
UINTERFACE()
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THEAPARTMENT_API IInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, Category="Interaction")
	void Interact(AVacancyPlayerCharacter* InteractingCharacter);

	UFUNCTION(BlueprintNativeEvent, Category="Interaction")
	void StartInteraction(AVacancyPlayerCharacter* InteractingCharacter);

	UFUNCTION(BlueprintNativeEvent, Category="Interaction")
	void EndInteraction(AVacancyPlayerCharacter* InteractingCharacter);

	UFUNCTION(BlueprintNativeEvent, Category="Interaction")
	FInteractionVisualInfo GetInteractionVisualInfo() const;

	UFUNCTION(BlueprintNativeEvent, Category="Interaction")
	TSubclassOf<UVacancyInteractionBase> GetInteractionClass() const;
	
	UFUNCTION(BlueprintNativeEvent, Category="Interaction")
	UVacancyInteractionBase* GetInteraction() const;

	UFUNCTION(BlueprintNativeEvent, Category="Interaction")
	USceneComponent* GetInteractionTransformComponent() const;
};
