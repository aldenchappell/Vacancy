// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Characters/Player/Tools/PlayerToolComponent.h"


UPlayerToolComponent::UPlayerToolComponent()
{

}

void UPlayerToolComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UPlayerToolComponent::EquipTool(const FPlayerToolAttachmentStateInfo& ToolAttachmentStateInfo)
{
	if (!ToolAttachmentStateInfo.AttachedTool)
	{
		UE_LOG(LogTemp, Warning, TEXT("EquipTool called with null AttachedTool."));
		return;
	}

	OnToolEquipped.Broadcast(ToolAttachmentStateInfo);
}

void UPlayerToolComponent::UnequipCurrentTool()
{
	if (!CurrentToolAttachmentState.AttachedTool)
	{
		UE_LOG(LogTemp, Warning, TEXT("UnequipCurrentTool called but no tool is currently equipped."));
		return;
	}

	CurrentToolAttachmentState.AttachedTool->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentToolAttachmentState.AttachedTool = nullptr;
	CurrentToolAttachmentState.bIsAttached = false;
}

bool UPlayerToolComponent::IsToolEquipped() const
{
	return GetEquippedTool() != nullptr && CurrentToolAttachmentState.bIsAttached;
}

AActor* UPlayerToolComponent::GetEquippedTool() const
{
	return CurrentToolAttachmentState.AttachedTool ? CurrentToolAttachmentState.AttachedTool : nullptr;
}