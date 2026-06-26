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
	const AVacancyPlayerCharacter* PlayerCharacter,
	const TSubclassOf<UBasePlayerProgressionComponent> ComponentClass)
{
	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetPlayerProgressionComponentByClass failed: PlayerCharacter is null."));
		return nullptr;
	}

	if (!ComponentClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetPlayerProgressionComponentByClass failed: ComponentClass is null."));
		return nullptr;
	}

	TArray<UBasePlayerProgressionComponent*> ProgressionComponents;
	PlayerCharacter->GetComponents<UBasePlayerProgressionComponent>(ProgressionComponents);

	if (ProgressionComponents.Num() == 0)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("GetPlayerProgressionComponentByClass failed: no progression components found on %s."),
			*GetNameSafe(PlayerCharacter)
		);

		return nullptr;
	}

	for (UBasePlayerProgressionComponent* ProgressionComponent : ProgressionComponents)
	{
		if (!IsValid(ProgressionComponent))
		{
			continue;
		}

		if (ProgressionComponent->IsA(ComponentClass))
		{
			return ProgressionComponent;
		}
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("GetPlayerProgressionComponentByClass failed: could not find progression component class %s on %s. Found %d progression component(s)."),
		*GetNameSafe(ComponentClass),
		*GetNameSafe(PlayerCharacter),
		ProgressionComponents.Num()
	);

	for (const UBasePlayerProgressionComponent* ProgressionComponent : ProgressionComponents)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT(" - Found progression component: %s | Class: %s"),
			*GetNameSafe(ProgressionComponent),
			*GetNameSafe(ProgressionComponent ? ProgressionComponent->GetClass() : nullptr)
		);
	}

	return nullptr;
}
