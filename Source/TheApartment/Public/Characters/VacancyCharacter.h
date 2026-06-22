// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VacancyCharacter.generated.h"

UCLASS()
class THEAPARTMENT_API AVacancyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AVacancyCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


};
