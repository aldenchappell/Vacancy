// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "VacancyCharacterAnimInstance.generated.h"

USTRUCT(BlueprintType)
struct FBasicGameplayStateInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayTagContainer ActiveGameplayTags;
};

UCLASS()
class THEAPARTMENT_API UVacancyCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UVacancyCharacterAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void UpdateVacancyAnimationState(const FBasicGameplayStateInfo& NewGameplayInfo);
};
