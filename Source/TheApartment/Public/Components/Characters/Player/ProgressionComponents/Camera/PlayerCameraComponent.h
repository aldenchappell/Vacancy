// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Characters/Player/ProgressionComponents/BasePlayerProgressionComponent.h"
#include "PlayerCameraComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEAPARTMENT_API UPlayerCameraComponent : public UBasePlayerProgressionComponent
{
	GENERATED_BODY()

public:
	
	UPlayerCameraComponent();

	void ToggleCamera(const bool bActive);

	bool TryTakePhoto() const;
protected:
	
	virtual void BeginPlay() override;


};
