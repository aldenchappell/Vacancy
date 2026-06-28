// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTool.h"
#include "RecorderTool.generated.h"

UCLASS()
class THEAPARTMENT_API ARecorderTool : public ABaseTool
{
	GENERATED_BODY()

public:
	ARecorderTool();

protected:
	virtual void BeginPlay() override;

	virtual void OnToolEquipped_Implementation(AVacancyPlayerCharacter* UnequippingCharacter) override;
	virtual void OnToolUnequipped_Implementation(AVacancyPlayerCharacter* UnequippingCharacter) override;	
};
