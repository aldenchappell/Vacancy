// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VacancyPlayerUtils.generated.h"

class UBasePlayerProgressionComponent;
class AVacancyPlayerCharacter;
/**
 * 
 */
UCLASS()
class THEAPARTMENT_API UVacancyPlayerUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static AVacancyPlayerCharacter* GetVacancyPlayerCharacter(const UObject* WorldContextObject);

	static UBasePlayerProgressionComponent* GetPlayerProgressionComponentByClass(
		const AVacancyPlayerCharacter* PlayerCharacter,
		const TSubclassOf<UBasePlayerProgressionComponent> ComponentClass);

	
};
