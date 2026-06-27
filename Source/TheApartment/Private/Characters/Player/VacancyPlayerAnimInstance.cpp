// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/VacancyPlayerAnimInstance.h"

#include "Characters/Player/VacancyPlayerCharacter.h"

void UVacancyPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningPlayerCharacter = Cast<AVacancyPlayerCharacter>(TryGetPawnOwner());
	if (!IsValid(OwningPlayerCharacter))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("UVacancyPlayerAnimInstance::NativeInitializeAnimation failed: owner is not AVacancyPlayerCharacter. Owner: %s"),
			*GetNameSafe(GetOwningActor()));
	}
}

void UVacancyPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	
}

void UVacancyPlayerAnimInstance::UpdatePlayerAnimProps(const FVacancyPlayerAnimData& NewAnimData)
{
	PlayerAnimData = NewAnimData;
	PlayerAnimDataUpdated();
}

void UVacancyPlayerAnimInstance::UpdateEquippedToolType(const EToolType NewToolType)
{
	PlayerAnimData.EquippedToolType = NewToolType;
	PlayerAnimDataUpdated();
}

void UVacancyPlayerAnimInstance::PlayerAnimDataUpdated()
{
	OnPlayerAnimDataUpdated.Broadcast(PlayerAnimData);
}