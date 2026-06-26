// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Characters/Player/ProgressionComponents/BasePlayerProgressionComponent.h"

#include "Components/Characters/Player/Tools/PlayerToolComponent.h"
#include "GameFramework/Character.h"
#include "HAL/IConsoleManager.h"
#include "Systems/Items/Tools/BaseTool.h"

static bool GVacancyProgressionComponentLogging = false;

static FAutoConsoleVariableRef CVarEnableProgressionComponentLoggingRef(
	TEXT("Vacancy.ProgressionComponent.EnableLogging"),
	GVacancyProgressionComponentLogging,
	TEXT("Enable logging for progression components."),
	ECVF_Cheat
);

static bool IsProgressionComponentLoggingEnabled()
{
	return GVacancyProgressionComponentLogging;
}

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

	ComponentToolState.bIsToolEnabled = bEnable;
}

bool UBasePlayerProgressionComponent::AttachToolToDesiredSocket()
{
	ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
	if (!IsValid(OwnerChar))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Owner is not a valid ACharacter."), *GetNameSafe(this));
		return false;
	}

	if (!ComponentToolState.bIsToolEnabled)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Cannot attach tool because it is not enabled."), *GetNameSafe(this));
		return false;
	}

	if (!ComponentToolState.ComponentToolClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: ComponentToolClass is not set."), *GetNameSafe(this));
		return false;
	}

	if (ComponentToolState.ComponentSocketAttachName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: ComponentSocketAttachName is not set."), *GetNameSafe(this));
		return false;
	}

	USkeletalMeshComponent* OwnerMesh = OwnerChar->GetMesh();
	if (!IsValid(OwnerMesh))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Owner character has no mesh."), *GetNameSafe(this));
		return false;
	}

	UPlayerToolComponent* PlayerToolComponent = OwnerChar->FindComponentByClass<UPlayerToolComponent>();
	if (!IsValid(PlayerToolComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Owner has no PlayerToolComponent."), *GetNameSafe(this));
		return false;
	}

	if (PlayerToolComponent->IsToolEquipped())
	{
		HandleToolAlreadyEquipped(true);
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerChar;
	SpawnParams.Instigator = OwnerChar;

	ABaseTool* NewToolInstance = GetWorld()->SpawnActor<ABaseTool>(
		ComponentToolState.ComponentToolClass,
		FTransform::Identity,
		SpawnParams
	);

	if (!IsValid(NewToolInstance))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("%s: Failed to spawn tool actor of class %s."),
			*GetNameSafe(this),
			*GetNameSafe(ComponentToolState.ComponentToolClass)
		);

		return false;
	}

	const bool bAttached = NewToolInstance->AttachToComponent(
		OwnerMesh,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		ComponentToolState.ComponentSocketAttachName
	);

	if (!bAttached)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("%s: Failed to attach tool actor to socket %s."),
			*GetNameSafe(this),
			*ComponentToolState.ComponentSocketAttachName.ToString()
		);

		NewToolInstance->Destroy();
		return false;
	}

	ComponentToolState.ComponentToolInstance = NewToolInstance;

	FPlayerToolAttachmentStateInfo NewStateInfo;
	ConstructNewProgressionInfo(NewStateInfo, NewToolInstance);
	NewToolInstance->SetToolAttachmentStateInfo(NewStateInfo);

	const bool bEquipped = PlayerToolComponent->EquipNewTool(NewStateInfo);
	if (!bEquipped)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("%s: Failed to equip tool actor. NewStateInfo: ProgressionComponentClass=%s, ToolClass=%s, ToolAttachSocket=%s, AttachedTool=%s, bIsAttached=%s"),
			*GetNameSafe(this),
			*GetNameSafe(NewStateInfo.ProgressionComponentClass),
			*GetNameSafe(NewStateInfo.ToolClass),
			*NewStateInfo.ToolAttachSocket.ToString(),
			*GetNameSafe(NewStateInfo.AttachedTool),
			NewStateInfo.bIsAttached ? TEXT("true") : TEXT("false")
		);

		NewToolInstance->Destroy();
		ComponentToolState.ComponentToolInstance = nullptr;
		return false;
	}

	return true;
}

void UBasePlayerProgressionComponent::UnlockState()
{
	if (ComponentToolState.bIsToolEnabled)
	{
		return;
	}

	ComponentToolState.bIsToolEnabled = true;

	OnComponentToolStateUnlocked();

	OnPlayerProgressionToolStateChanged.Broadcast(ComponentToolState);
}

bool UBasePlayerProgressionComponent::EquipProgressionComponentTool()
{
	if (!ComponentToolState.bIsToolEnabled)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("%s: Cannot equip progression tool because it is not unlocked/enabled."),
			*GetNameSafe(this)
		);

		return false;
	}

	return AttachToolToDesiredSocket();
}

void UBasePlayerProgressionComponent::UnlockProgressionComponentTool()
{
	if (!IsValid(GetOwner()))
	{
		UE_LOG(LogTemp, Warning, TEXT("UnlockProgressionComponentTool failed: component has no valid owner."));
		return;
	}

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

bool UBasePlayerProgressionComponent::DebugState()
{
	return IsProgressionComponentLoggingEnabled();
}

void UBasePlayerProgressionComponent::DetachToolFromSocket(const AActor* ToolActor)
{
	
}

void UBasePlayerProgressionComponent::HandleToolAlreadyEquipped(const bool bForceSwap)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!IsValid(OwnerCharacter))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("%s: Cannot handle equipped tool because owner is not a valid character."),
			*GetName()
		);
		return;
	}

	UPlayerToolComponent* PlayerToolComponent = OwnerCharacter->FindComponentByClass<UPlayerToolComponent>();
	if (!IsValid(PlayerToolComponent))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("%s: Cannot handle equipped tool because PlayerToolComponent is missing."),
			*GetName()
		);
		return;
	}

	if (bForceSwap)
	{
		PlayerToolComponent->UnequipCurrentTool();
		return;
	}

	if (DebugState())
	{
		UE_LOG(LogTemp, Log, TEXT("%s: Tool is already equipped. No action taken."), *GetName());
	}
}

void UBasePlayerProgressionComponent::ConstructNewProgressionInfo(
FPlayerToolAttachmentStateInfo& NewToolState, ABaseTool*& NewToolInstance) const
{
	if (!IsValid(NewToolInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Cannot construct new progression info because NewToolInstance is null."), *GetName());
		return;
	}
	
	NewToolState.ProgressionComponentClass = GetClass();
	NewToolState.ToolClass = ComponentToolState.ComponentToolClass;
	NewToolState.AttachedTool = NewToolInstance;
	NewToolState.bIsAttached = true;
	NewToolState.ToolAttachSocket = ComponentToolState.ComponentSocketAttachName;
}
