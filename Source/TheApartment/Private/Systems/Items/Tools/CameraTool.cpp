// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Items/Tools/CameraTool.h"

ACameraTool::ACameraTool()
{
	ToolData.ToolType = EToolType::Camera;
	ToolData.ToolDisplayName = FName("Camera");
	ToolData.ToolDescription = FText::FromString("A camera tool for capturing evidence.");
}

void ACameraTool::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACameraTool::OnToolEquipped_Implementation(AVacancyPlayerCharacter* UnequippingCharacter)
{
	Super::OnToolEquipped_Implementation(UnequippingCharacter);

	
}

void ACameraTool::OnToolUnequipped_Implementation(AVacancyPlayerCharacter* UnequippingCharacter)
{
	Super::OnToolUnequipped_Implementation(UnequippingCharacter);
}


