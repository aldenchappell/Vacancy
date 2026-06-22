// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/VacancyPlayerController.h"

#include "UI/VacancyHUD.h"

AVacancyHUD* AVacancyPlayerController::GetVacancyHUD() const
{
	if (AHUD* CurrentHUD = GetHUD())
	{
		return Cast<AVacancyHUD>(CurrentHUD);
	}
	
	return nullptr;
}
