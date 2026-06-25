// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Characters/Player/Interaction/InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "VacancyInteractableBase.generated.h"

class UWidgetComponent;
class UBoxComponent;

UCLASS()
class THEAPARTMENT_API AVacancyInteractableBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	
	AVacancyInteractableBase();

	virtual void Interact_Implementation(AVacancyPlayerCharacter* InteractingCharacter) override;
	virtual void StartInteraction_Implementation(AVacancyPlayerCharacter* InteractingCharacter) override;
	virtual void EndInteraction_Implementation(AVacancyPlayerCharacter* InteractingCharacter) override;
	virtual UVacancyInteractionBase* GetInteraction_Implementation() const override;
	virtual TSubclassOf<UVacancyInteractionBase> GetInteractionClass_Implementation() const override;
	virtual FInteractionVisualInfo GetInteractionVisualInfo_Implementation() const override;
	virtual USceneComponent* GetInteractionTransformComponent_Implementation() const override;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	USceneComponent* InteractionTransformComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> InteractionCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	TSubclassOf<UVacancyInteractionBase> InteractionClass;

private:

	UPROPERTY()
	TObjectPtr<UVacancyInteractionBase> InteractionPtr = nullptr;
};
