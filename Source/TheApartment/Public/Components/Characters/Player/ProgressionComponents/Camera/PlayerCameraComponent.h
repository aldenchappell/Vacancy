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

	UFUNCTION(BlueprintCallable, Category = "Camera")
	bool SetCameraModeEnabled(bool bEnabled);

	UFUNCTION(BlueprintPure, Category = "Camera")
	bool CanEnterCameraMode() const;

	UFUNCTION(BlueprintCallable, Category = "Camera")
	bool TryTakePhoto();

	UFUNCTION(BlueprintPure, Category = "Camera")
	bool CanTakePhoto() const;
protected:
	
	virtual void BeginPlay() override;


};
