// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Characters/Player/ProgressionComponents/BasePlayerProgressionComponent.h"
#include "PlayerRecorderComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEAPARTMENT_API UPlayerRecorderComponent : public UBasePlayerProgressionComponent
{
	GENERATED_BODY()

public:
	UPlayerRecorderComponent();

	bool TryToggleRecorder(const bool bIsRecorderOn);

	UFUNCTION(BlueprintCallable, Category = "Recorder")
	bool SetRecordingEnabled(bool bEnabled);

	UFUNCTION(BlueprintPure, Category = "Recorder")
	bool CanStartRecording() const;
protected:
	
	virtual void BeginPlay() override;

};
