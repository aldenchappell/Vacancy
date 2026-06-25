// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTool.h"
#include "CameraTool.generated.h"

UCLASS()
class THEAPARTMENT_API ACameraTool : public ABaseTool
{
	GENERATED_BODY()

public:
	ACameraTool();

protected:
	virtual void BeginPlay() override;
	
};
