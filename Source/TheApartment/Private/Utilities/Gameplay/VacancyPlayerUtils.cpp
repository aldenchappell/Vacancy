// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/Gameplay/VacancyPlayerUtils.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Components/Characters/Player/ProgressionComponents/BasePlayerProgressionComponent.h"

AVacancyPlayerCharacter* UVacancyPlayerUtils::GetVacancyPlayerCharacter(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetVacancyPlayerCharacter called with null WorldContextObject."));
		return nullptr;
	}

	const UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetVacancyPlayerCharacter could not get UWorld from WorldContextObject."));
		return nullptr;
	}

	const APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetVacancyPlayerCharacter could not find a PlayerController in the world."));
		return nullptr;
	}

	return Cast<AVacancyPlayerCharacter>(PlayerController->GetPawn());
}

UBasePlayerProgressionComponent* UVacancyPlayerUtils::GetPlayerProgressionComponentByClass(
	const AVacancyPlayerCharacter* PlayerCharacter, const TSubclassOf<UBasePlayerProgressionComponent> ComponentClass)
{
	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetPlayerProgressionComponentByClass called with null PlayerCharacter."));
		return nullptr;
	}

	if (!ComponentClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetPlayerProgressionComponentByClass called with null ComponentClass."));
		return nullptr;
	}

	if (UBasePlayerProgressionComponent* FoundComponent = PlayerCharacter->FindComponentByClass<UBasePlayerProgressionComponent>())
	{
		if (FoundComponent->IsA(ComponentClass))
		{
			return FoundComponent;
		}
	}
	
	return nullptr;
}
