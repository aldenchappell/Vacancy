// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/Interaction/Interactables/VacancyInteractableBase.h"
#include "BaseInteractableTool.generated.h"

UCLASS()
class THEAPARTMENT_API ABaseInteractableTool : public AVacancyInteractableBase
{
	GENERATED_BODY()

public:
	
	ABaseInteractableTool();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tool")
	UStaticMeshComponent* ToolMesh;
	
};
