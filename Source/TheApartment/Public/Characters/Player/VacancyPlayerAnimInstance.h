// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VacancyPlayerAnimData.h"
#include "Characters/VacancyCharacterAnimInstance.h"

#include "VacancyPlayerAnimInstance.generated.h"

class AVacancyCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerAnimDataUpdated, const FVacancyPlayerAnimData&, NewAnimData);
UCLASS()
class THEAPARTMENT_API UVacancyPlayerAnimInstance : public UVacancyCharacterAnimInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation")
	FVacancyPlayerAnimData PlayerAnimData;

	void UpdatePlayerAnimProps(const FVacancyPlayerAnimData& NewAnimData);
	void UpdateEquippedToolType(const EToolType NewToolType);
protected:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable, Category="Animation")
	FOnPlayerAnimDataUpdated OnPlayerAnimDataUpdated;

private:

	UPROPERTY()
	AVacancyCharacter* OwningPlayerCharacter = nullptr;

	void PlayerAnimDataUpdated();
};
