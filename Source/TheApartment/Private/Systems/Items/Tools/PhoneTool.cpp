// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Items/Tools/PhoneTool.h"

APhoneTool::APhoneTool()
{
	ToolData.ToolDisplayName = FName("Phone");
	ToolData.ToolType = EToolType::Phone;
	ToolData.ToolDescription = FText::FromString("A smartphone used for communication and various tasks.");
}

void APhoneTool::BeginPlay()
{
	Super::BeginPlay();
	
}

void APhoneTool::OnToolEquipped_Implementation(AVacancyPlayerCharacter* UnequippingCharacter)
{
	Super::OnToolEquipped_Implementation(UnequippingCharacter);
}

void APhoneTool::OnToolUnequipped_Implementation(AVacancyPlayerCharacter* UnequippingCharacter)
{
	Super::OnToolUnequipped_Implementation(UnequippingCharacter);
}
