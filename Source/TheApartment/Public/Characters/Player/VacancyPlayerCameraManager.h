// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "VacancyPlayerCameraManager.generated.h"

UENUM(BlueprintTYpe)
enum class EVacancyCameraMode : uint8
{
	FirstPerson,
	ThirdPerson,
	Surveillance
};

UCLASS()
class THEAPARTMENT_API AVacancyPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AVacancyPlayerCameraManager();
	
	void SwapCameraMode(EVacancyCameraMode NewCameraMode);

private:

	void SwitchCameraMode(EVacancyCameraMode NewCameraMode);
	EVacancyCameraMode CurrentCameraMode = EVacancyCameraMode::FirstPerson;
};
