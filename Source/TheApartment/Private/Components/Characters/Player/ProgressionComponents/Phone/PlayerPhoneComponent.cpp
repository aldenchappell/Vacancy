// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Characters/Player/ProgressionComponents/Phone/PlayerPhoneComponent.h"


UPlayerPhoneComponent::UPlayerPhoneComponent()
{

}

bool UPlayerPhoneComponent::SetPhoneOpen(bool bOpen)
{
	return true;
}

bool UPlayerPhoneComponent::CanOpenPhone() const
{
	return true;
}

void UPlayerPhoneComponent::BeginPlay()
{
	Super::BeginPlay();

}

