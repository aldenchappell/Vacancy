// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Characters/Player/Tools/PlayerToolComponent.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Systems/Items/Tools/BaseTool.h"
#include "Utilities/Gameplay/VacancyPlayerUtils.h"


UPlayerToolComponent::UPlayerToolComponent()
{
	
}

void UPlayerToolComponent::EquipNewTool(const FPlayerToolAttachmentStateInfo& NewToolState)
{
	if (!IsValid(NewToolState.AttachedTool))
	{
		UE_LOG(LogTemp, Warning, TEXT("EquipNewTool called with null NewTool."));
		return;
	}

	AddSpawnedTool(NewToolState.AttachedTool);
	UpdateCurrentAttachmentState(NewToolState);
}

void UPlayerToolComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningPlayerCharacter = UVacancyPlayerUtils::GetVacancyPlayerCharacter(GetOwner());
	if (!OwningPlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPlayerToolComponent::BeginPlay - OwningPlayerCharacter is null."));
	}
}

void UPlayerToolComponent::UnequipCurrentTool()
{
	if (!CurrentToolAttachmentState.AttachedTool || !GetEquippedTool())
	{
		UE_LOG(LogTemp, Warning, TEXT("UnequipCurrentTool called but no tool is currently equipped."));
		return;
	}

	if (!OwningPlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("UnequipCurrentTool called but OwningPlayerCharacter is null."));
		return;
	}

	CurrentToolAttachmentState.AttachedTool->OnToolUnequipped(OwningPlayerCharacter);
	CurrentToolAttachmentState.AttachedTool->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentToolAttachmentState.AttachedTool = nullptr;
	CurrentToolAttachmentState.bIsAttached = false;

	RemoveSpawnedTool(CurrentToolAttachmentState.AttachedTool);
}

void UPlayerToolComponent::UpdateCurrentAttachmentState(const FPlayerToolAttachmentStateInfo& NewAttachmentState)
{
	if (!NewAttachmentState.AttachedTool)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateCurrentAttachmentState called with null AttachedTool."));
		return;
	}

	if (!NewAttachmentState.ToolClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateCurrentAttachmentState called with null ToolClass."));
		return;
	}
	
	CurrentToolAttachmentState = NewAttachmentState;
}

void UPlayerToolComponent::AddSpawnedTool(ABaseTool* NewSpawnedTool)
{
	if (!IsValid(NewSpawnedTool))
	{
		UE_LOG(LogTemp, Warning, TEXT("AddSpawnedTool called with null NewSpawnedTool."));
		return;
	}

	SpawnedTools.Add(NewSpawnedTool);
}

void UPlayerToolComponent::RemoveSpawnedTool(ABaseTool* NewSpawnedTool)
{
	if (!IsValid(NewSpawnedTool))
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveSpawnedTool called with null NewSpawnedTool."));
		return;
	}

	SpawnedTools.Remove(NewSpawnedTool);
	
	// If the removed tool was the currently equipped tool, clear the current attachment state
	if (CurrentToolAttachmentState.AttachedTool == NewSpawnedTool)
	{
		CurrentToolAttachmentState.Clear();
	}
}

bool UPlayerToolComponent::AttachToolToSocket(const FPlayerToolAttachmentStateInfo& NewToolState)
{
	if (!IsValid(NewToolState.AttachedTool))
	{
		UE_LOG(LogTemp, Warning, TEXT("AttachToolToSocket called with null AttachedTool."));
		return false;
	}

	if (!OwningPlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttachToolToSocket called but OwningPlayerCharacter is null."));
		return false;
	}

	const FName SocketName = NewToolState.AttachedTool->GetToolAttachSocket();
	if (SocketName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("AttachToolToSocket called but AttachedTool has no valid socket name."));
		return false;
	}

	return OwningPlayerCharacter->GetMesh()->AttachToComponent(
		NewToolState.AttachedTool->GetRootComponent(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		SocketName
	);
}

bool UPlayerToolComponent::IsToolEquipped() const
{
	return GetEquippedTool() != nullptr && CurrentToolAttachmentState.bIsAttached;
}

ABaseTool* UPlayerToolComponent::GetEquippedTool() const
{
	return CurrentToolAttachmentState.AttachedTool ? CurrentToolAttachmentState.AttachedTool : nullptr;
}