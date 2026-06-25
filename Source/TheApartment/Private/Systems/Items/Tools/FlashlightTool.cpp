// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Items/Tools/FlashlightTool.h"

#include "Components/SpotLightComponent.h"


AFlashlightTool::AFlashlightTool()
{
	FlashlightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashlightComponent"));
	FlashlightComponent->SetupAttachment(ToolMesh);

	FlashlightComponent->SetInnerConeAngle(FlashlightSettings.InnerConeAngle);
	FlashlightComponent->SetOuterConeAngle(FlashlightSettings.OuterConeAngle);
	FlashlightComponent->SetIntensity(FlashlightSettings.Intensity);
	FlashlightComponent->SetAttenuationRadius(FlashlightSettings.AttenuationRadius);
	FlashlightComponent->SetLightColor(FlashlightSettings.LightColor);
}

void AFlashlightTool::BeginPlay()
{
	Super::BeginPlay();
	
}