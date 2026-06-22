// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/VacancyPlayerCameraManager.h"

AVacancyPlayerCameraManager::AVacancyPlayerCameraManager()
{
	CurrentCameraMode = EVacancyCameraMode::FirstPerson; // Default camera mode
}

void AVacancyPlayerCameraManager::SwapCameraMode(const EVacancyCameraMode NewCameraMode)
{
	if (CurrentCameraMode == NewCameraMode)
	{
		return; // No need to swap if the new mode is the same as the current mode
	}

	SwitchCameraMode(NewCameraMode);
	
	CurrentCameraMode = NewCameraMode;
}

void AVacancyPlayerCameraManager::SwitchCameraMode(EVacancyCameraMode NewCameraMode)
{
	switch (NewCameraMode)
	{
	case EVacancyCameraMode::FirstPerson:
		// Set up first-person camera settings
		break;
	case EVacancyCameraMode::ThirdPerson:
		// Set up third-person camera settings
		break;
	case EVacancyCameraMode::Surveillance:
		// Set up surveillance camera settings
		break;
	default:
		break;
	}
}
