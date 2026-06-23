// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Characters/Player/ProgressionComponents/BasePlayerProgressionComponent.h"
#include "PlayerPhoneComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEAPARTMENT_API UPlayerPhoneComponent : public UBasePlayerProgressionComponent
{
	GENERATED_BODY()

public:
	UPlayerPhoneComponent();

	bool TryTogglePhone(const bool bActive);

protected:
	
	virtual void BeginPlay() override;

};
