// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Characters/Player/Tools/PlayerToolComponent.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Systems/Items/Tools/BaseTool.h"
#include "Utilities/Gameplay/VacancyPlayerUtils.h"


UPlayerToolComponent::UPlayerToolComponent()
{
	
}

bool UPlayerToolComponent::EquipNewTool(const FPlayerToolAttachmentStateInfo& NewToolState)
{
	if (!IsValid(NewToolState.AttachedTool))
	{
		UE_LOG(LogTemp, Warning, TEXT("EquipNewTool called with null NewTool."));
		return false;
	}

	AddSpawnedTool(NewToolState.AttachedTool);
	UpdateCurrentAttachmentState(NewToolState);

	return true;
}

void UPlayerToolComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningPlayerCharacter = Cast<AVacancyPlayerCharacter>(GetOwner());

	if (!IsValid(OwningPlayerCharacter))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("UPlayerToolComponent::BeginPlay failed: owner is not AVacancyPlayerCharacter. Owner: %s"),
			*GetNameSafe(GetOwner())
		);
	}
}

void UPlayerToolComponent::UnequipCurrentTool()
{
	ABaseTool* ToolToUnequip = CurrentToolAttachmentState.AttachedTool;

	if (!IsValid(ToolToUnequip))
	{
		UE_LOG(LogTemp, Warning, TEXT("UnequipCurrentTool called but no valid tool is currently equipped."));
		return;
	}

	if (!IsValid(OwningPlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("UnequipCurrentTool called but OwningPlayerCharacter is null."));
		return;
	}

	ToolToUnequip->OnToolUnequipped(OwningPlayerCharacter);
	ToolToUnequip->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	RemoveSpawnedTool(ToolToUnequip);

	FPlayerToolAttachmentStateInfo::Clear(CurrentToolAttachmentState);
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
	NewSpawnedTool->OnToolEquipped(OwningPlayerCharacter);

	OwningPlayerCharacter->UpdateAnimPropsForEquippedTool(NewSpawnedTool);
}

void UPlayerToolComponent::RemoveSpawnedTool(ABaseTool* NewSpawnedTool)
{
	if (!IsValid(NewSpawnedTool))
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveSpawnedTool called with null NewSpawnedTool."));
		return;
	}

	SpawnedTools.Remove(NewSpawnedTool);
}

bool UPlayerToolComponent::AttachToolToSocket(const FPlayerToolAttachmentStateInfo& NewToolState)
{
	if (!IsValid(NewToolState.AttachedTool))
	{
		UE_LOG(LogTemp, Warning, TEXT("AttachToolToSocket called with null AttachedTool."));
		return false;
	}

	if (!IsValid(OwningPlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("AttachToolToSocket called but OwningPlayerCharacter is null."));
		return false;
	}

	USkeletalMeshComponent* PlayerMesh = OwningPlayerCharacter->GetMesh();
	if (!IsValid(PlayerMesh))
	{
		UE_LOG(LogTemp, Warning, TEXT("AttachToolToSocket called but player mesh is null."));
		return false;
	}

	if (NewToolState.ToolAttachSocket.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("AttachToolToSocket called but ToolAttachSocket is None."));
		return false;
	}

	return NewToolState.AttachedTool->AttachToComponent(
		PlayerMesh,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		NewToolState.ToolAttachSocket
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