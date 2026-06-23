// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Characters/Player/Evidence/EvidenceInventoryComponent.h"


UEvidenceInventoryComponent::UEvidenceInventoryComponent()
{

}

void UEvidenceInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

bool UEvidenceInventoryComponent::TryToggleInventory(const bool bOpenInventory)
{
	return false;
}

