// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InteractableUtils.generated.h"

class UVacancyInteractionBase;
/**
 * 
 */
UCLASS()
class THEAPARTMENT_API UInteractableUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(Category="Interaction")
	static UVacancyInteractionBase* GetInteractionFromActor(const AActor* InActor);

	UFUNCTION(Category="Interaction")
	static bool ConstructInteractionFromActor(AActor* InActor, UVacancyInteractionBase*& OutInteraction);

	UFUNCTION(Category="Interaction")
	static FTransform GetInteractionTransformFromActor(const AActor* InActor);

	UFUNCTION(Category="Interaction")
	static UVacancyInteractionBase* GetInteractionFromIdentifier(const AActor* InActor, const FName& InteractionIdentifier);

	UFUNCTION(Category="Interaction")
	static AActor* GetInteractableActorFromInteraction(const UVacancyInteractionBase* Interaction);

	
};
