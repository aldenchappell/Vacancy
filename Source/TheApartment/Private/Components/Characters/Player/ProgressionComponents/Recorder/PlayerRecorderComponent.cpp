// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Characters/Player/ProgressionComponents/Recorder/PlayerRecorderComponent.h"


UPlayerRecorderComponent::UPlayerRecorderComponent()
{

}

void UPlayerRecorderComponent::BeginPlay()
{
	Super::BeginPlay();

}

bool UPlayerRecorderComponent::TryToggleRecorder(const bool bIsRecorderOn)
{
	return false;
}