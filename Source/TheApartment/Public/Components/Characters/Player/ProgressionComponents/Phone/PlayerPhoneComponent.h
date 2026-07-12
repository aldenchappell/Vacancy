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

	UFUNCTION(BlueprintCallable, Category = "Phone")
	bool SetPhoneOpen(bool bOpen);

	UFUNCTION(BlueprintPure, Category = "Phone")
	bool CanOpenPhone() const;

protected:
	
	virtual void BeginPlay() override;

};
