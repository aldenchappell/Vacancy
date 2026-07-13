// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Characters/Player/ProgressionComponents/Camera/PlayerCameraComponent.h"


UPlayerCameraComponent::UPlayerCameraComponent()
{
	
}

void UPlayerCameraComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UPlayerCameraComponent::ToggleCamera(const bool bActive)
{
	
}

bool UPlayerCameraComponent::SetCameraModeEnabled(bool bEnabled)
{
	return true;
}

bool UPlayerCameraComponent::CanEnterCameraMode() const
{
	return true;
}

bool UPlayerCameraComponent::TryTakePhoto()
{
	return true;
}

bool UPlayerCameraComponent::CanTakePhoto() const
{
	return true;
}

