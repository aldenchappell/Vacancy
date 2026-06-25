// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Characters/Player/ProgressionComponents/BasePlayerProgressionComponent.h"

#include "Components/Characters/Player/Tools/PlayerToolComponent.h"
#include "GameFramework/Character.h"
#include "Systems/Items/Tools/BaseTool.h"

static TAutoConsoleVariable<bool> CVarEnableProgressionComponentLogging(
	TEXT("Vacancy.ProgressionComponent.EnableLogging"),
	false,
	TEXT("Enable logging for progression components."),
	ECVF_Cheat);

UBasePlayerProgressionComponent::UBasePlayerProgressionComponent()
{

}

void UBasePlayerProgressionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ComponentToolState.bStartUnlocked)
	{
		UnlockProgressionComponentTool();
	}
}

void UBasePlayerProgressionComponent::ToggleComponentToolState(const bool bEnable)
{
	Internal_ToggleComponentToolState(bEnable);
}

void UBasePlayerProgressionComponent::Internal_ToggleComponentToolState(const bool bEnable)
{
	if (ComponentToolState.bIsToolEnabled == bEnable)
	{
		return; // No change needed, already in the desired state
	}

	
}

bool UBasePlayerProgressionComponent::AttachToolToDesiredSocket()
{
	if (!ComponentToolState.bIsToolEnabled)
	{
		if (DebugState())
		{
			UE_LOG(LogTemp, Log, TEXT("%s: Cannot attach tool to socket because the tool is not enabled."), *GetName());
		}
		return false; // Tool is not enabled, cannot attach
	}

	if (!ComponentToolState.ComponentToolClass)
	{
		if (DebugState())
		{
			UE_LOG(LogTemp, Log, TEXT("%s: Cannot attach tool to socket because the ComponentToolClass is not set."), *GetName());
		}
		return false; // Tool class is not set, cannot attach
	}

	if (!ComponentToolState.ComponentSocketAttachName.IsValid())
	{
		if (DebugState())
		{
			UE_LOG(LogTemp, Log, TEXT("%s: Cannot attach tool to socket because the ComponentSocketAttachName is not valid."), *GetName());
		}
		return false; // Socket name is not valid, cannot attach
	}
	
	if (const ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
	{
		USkeletalMeshComponent* OwnerMesh = OwnerChar->GetMesh();
		if (!OwnerMesh)
		{
			if (DebugState())
			{
				UE_LOG(LogTemp, Log, TEXT("%s: Cannot attach tool to socket because the owner character has no mesh."), *GetName());
			}
			return false; // Owner has no mesh, cannot attach
		}

		//check to see if we already have a tool equipped.
		UPlayerToolComponent* PlayerToolComponent = OwnerChar->GetComponentByClass<UPlayerToolComponent>();
		if (!PlayerToolComponent)
		{
			if (DebugState())
			{
				UE_LOG(LogTemp, Log, TEXT("%s: Cannot attach tool to socket because the owner character has no PlayerToolComponent."), *GetName());
			}
			return false; // Owner has no PlayerToolComponent, cannot attach
		}

		if (PlayerToolComponent->IsToolEquipped())
		{
			HandleToolAlreadyEquipped();
			return false; // Tool is already equipped, cannot attach
		}

		// Spawn the tool actor
		ABaseTool* NewToolInstance = GetWorld()->SpawnActor<ABaseTool>(ComponentToolState.ComponentToolClass);
		if (!NewToolInstance)
		{
			if (DebugState())
			{
				UE_LOG(LogTemp, Log, TEXT("%s: Failed to spawn tool actor of class %s."), *GetName(), *GetNameSafe(ComponentToolState.ComponentToolClass));
			}
			return false; // Failed to spawn tool actor
		}

		ComponentToolState.ComponentToolInstance = NewToolInstance;
		
		// Attach the tool to the specified socket
		const bool Attached = NewToolInstance->AttachToComponent(
			OwnerMesh,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			ComponentToolState.ComponentSocketAttachName);

		if (!Attached)
		{
			if (DebugState())
			{
				UE_LOG(LogTemp, Log, TEXT("%s: Failed to attach tool actor to socket %s."), *GetName(), *ComponentToolState.ComponentSocketAttachName.ToString());
			}
			NewToolInstance->Destroy(); // Clean up the spawned actor
			ComponentToolState.ComponentToolInstance = nullptr;
			return false; // Failed to attach
		}

		// Equip the tool using the PlayerToolComponent
		PlayerToolComponent->EquipNewTool(NewToolInstance);
	}
	
	// Successfully attached the tool to the socket
	return true; 
}

void UBasePlayerProgressionComponent::UnlockState()
{
	ComponentToolState.bIsToolEnabled = true;
	OnComponentToolStateUnlocked();
}

void UBasePlayerProgressionComponent::UnlockProgressionComponentTool()
{
	if (DebugState())
	{
		UE_LOG(LogTemp, Log, TEXT("%s: Unlocking progression component tool."), *GetName());
	}
	
	UnlockState();
}

void UBasePlayerProgressionComponent::OnComponentToolStateUnlocked_Implementation()
{
	
}

void UBasePlayerProgressionComponent::OnComponentToolStateInitialized_Implementation()
{
	
}

bool UBasePlayerProgressionComponent::DebugState() const
{
	return CVarEnableProgressionComponentLogging.GetValueOnGameThread();
}

void UBasePlayerProgressionComponent::DetachToolFromSocket(const AActor* ToolActor)
{
	
}

void UBasePlayerProgressionComponent::HandleToolAlreadyEquipped(const bool bForceSwap)
{
	if (bForceSwap)
	{
		if (DebugState())
		{
			UE_LOG(LogTemp, Log, TEXT("%s: Forcing tool swap."), *GetName());
		}

		if (UPlayerToolComponent* PlayerToolComponent = Cast<ACharacter>(GetOwner())->FindComponentByClass<UPlayerToolComponent>())
		{
			PlayerToolComponent->UnequipCurrentTool();
			AttachToolToDesiredSocket();
		}
		else
		{
			if (DebugState())
			{
				UE_LOG(LogTemp, Warning, TEXT("%s: Cannot force tool swap because PlayerToolComponent is not found."), *GetName());
			}
		}
	}
	else
	{
		//TODO: Implement logic to handle the case where the tool is already equipped and we do not want to force a swap.
		//Send a message to the player that the tool is already equipped and cannot be equipped again.
		if (DebugState())
		{
			UE_LOG(LogTemp, Log, TEXT("%s: Tool is already equipped. No action taken."), *GetName());
		}
	}
}