// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Characters/Player/ProgressionComponents/BasePlayerProgressionComponent.h"
#include "PlayerFlashlightComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEAPARTMENT_API UPlayerFlashlightComponent : public UBasePlayerProgressionComponent
{
	GENERATED_BODY()

public:
	UPlayerFlashlightComponent();

	void ToggleFlashlight(const bool bActive);

protected:
	
	virtual void BeginPlay() override;

};
