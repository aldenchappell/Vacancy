// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTool.h"
#include "FlashlightTool.generated.h"

class USpotLightComponent;

USTRUCT(BlueprintType)
struct FFlashlightSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flashlight Settings")
	float Intensity = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flashlight Settings")
	float AttenuationRadius = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flashlight Settings")
	float InnerConeAngle = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flashlight Settings")
	float OuterConeAngle = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flashlight Settings")
	FColor LightColor = FColor::White;
};

UCLASS()
class THEAPARTMENT_API AFlashlightTool : public ABaseTool
{
	GENERATED_BODY()

public:
	AFlashlightTool();

protected:
	virtual void BeginPlay() override;

	virtual void OnToolEquipped_Implementation(AVacancyPlayerCharacter* UnequippingCharacter) override;
	virtual void OnToolUnequipped_Implementation(AVacancyPlayerCharacter* UnequippingCharacter) override;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flashlight")
	USpotLightComponent* FlashlightComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Flashlight")
	FFlashlightSettings FlashlightSettings;
};
