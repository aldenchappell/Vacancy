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

	ToolData.ToolType = EToolType::Flashlight;
	ToolData.ToolDisplayName = FName("Flashlight");
	ToolData.ToolDescription = FText::FromString("A flashlight to illuminate dark areas.");
}

void AFlashlightTool::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFlashlightTool::OnToolEquipped_Implementation(AVacancyPlayerCharacter* UnequippingCharacter)
{
	Super::OnToolEquipped_Implementation(UnequippingCharacter);
}

void AFlashlightTool::OnToolUnequipped_Implementation(AVacancyPlayerCharacter* UnequippingCharacter)
{
	Super::OnToolUnequipped_Implementation(UnequippingCharacter);
}
