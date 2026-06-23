// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EvidenceInventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEAPARTMENT_API UEvidenceInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEvidenceInventoryComponent();

	bool TryToggleInventory(const bool bOpenInventory);

protected:
	
	virtual void BeginPlay() override;

};
