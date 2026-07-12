// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Items/Tools/BaseTool.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Systems/AbilitySystem/VacancyAbilitySystemComponent.h"
#include "Systems/AbilitySystem/Abilities/Data/VacancyAbilityData.h"
#include "Systems/AbilitySystem/Abilities/ToolAbilities/BaseToolAbility.h"
#include "UI/VacancyHUDData.h"

static TAutoConsoleVariable<int32> CVarDebugTools(
	TEXT("Vacancy.DebugTools"),
	0,
	TEXT("0: Disable Vacancy tool logs.\n")
	TEXT("1: Enable Vacancy tool logs."),
	ECVF_Default);

static bool bDebugTools = false;


ABaseTool::ABaseTool()
{
	ToolMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Tool Mesh"));
	SetRootComponent(ToolMesh);

	ToolMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ToolMesh->SetCollisionResponseToAllChannels(ECR_Block);

	//enable physics simulation for the tool mesh
	ToolMesh->SetSimulatePhysics(true);
}

void ABaseTool::SetToolAttachmentStateInfo(const FPlayerToolAttachmentStateInfo& NewToolAttachmentStateInfo)
{
	if (!IsValid(NewToolAttachmentStateInfo.AttachedTool))
	{
		if (DebugTools())
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("SetToolAttachmentStateInfo called with null AttachedTool for tool %s."),
				*GetNameSafe(this));
		}
		
		return;
	}

	if (!IsValid(NewToolAttachmentStateInfo.ToolClass))
	{
		if (DebugTools())
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("SetToolAttachmentStateInfo called with null ToolClass for tool %s."),
				*GetNameSafe(NewToolAttachmentStateInfo.AttachedTool));
		}

		return;
	}

	ToolAttachmentStateInfo = NewToolAttachmentStateInfo;
}

void ABaseTool::BeginPlay()
{
	Super::BeginPlay();
	
}


void ABaseTool::OnToolEquipped_Implementation(AVacancyPlayerCharacter* UnequippingCharacter)
{
	if (IsValid(UnequipToolAnim) && IsValid(UnequippingCharacter))
	{
		UnequippingCharacter->StopAnimMontage(UnequipToolAnim);
	}
	
	if (IsValid(EquipToolAnim) && IsValid(UnequippingCharacter))
	{
		UnequippingCharacter->PlayAnimMontage(EquipToolAnim);
	}

	const float MontageDuration = IsValid(EquipToolAnim) ? EquipToolAnim->GetPlayLength() : 0.f;
	FTimerHandle EquipTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
	EquipTimerHandle,
		[this, UnequippingCharacter]()
		{
			if (IsValid(UnequippingCharacter))
			{
				UnequippingCharacter->UpdateHUDByType(EVacancyHUDElementType::ToolHUD);
			}
		},
		MontageDuration,
		false
	);
	// Additional logic for when the tool is equipped will be implemented in derived classes.
}

void ABaseTool::OnToolUnequipped_Implementation(AVacancyPlayerCharacter* UnequippingCharacter)
{
	if (IsValid(EquipToolAnim) && IsValid(UnequippingCharacter))
	{
		UnequippingCharacter->StopAnimMontage(EquipToolAnim);
	}

	if (IsValid(UnequipToolAnim) && IsValid(UnequippingCharacter))
	{
		UnequippingCharacter->PlayAnimMontage(UnequipToolAnim);
	}
}

FName ABaseTool::GetToolAttachSocket() const
{
	return ToolAttachmentStateInfo.ToolAttachSocket;
}

TArray<FVacancyAbilityData> ABaseTool::GetToolAbilityDatas() const
{
	if (ToolData.ToolAbilityClasses.IsEmpty())
	{
		if (DebugTools())
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("%s does not have any ToolAbilityClasses."),
				*GetNameSafe(this));
		}

		return TArray<FVacancyAbilityData>();
	}

	for (const TSubclassOf<UBaseToolAbility>& AbilityClass : ToolData.ToolAbilityClasses)
	{
		if (!IsValid(AbilityClass))
		{
			if (DebugTools())
			{
				UE_LOG(
					LogTemp,
					Warning,
					TEXT("%s contains a null tool ability class."),
					*GetNameSafe(this));
			}

			continue;
		}

		const FVacancyAbilityData* AbilityData = AbilityClass->GetDefaultObject<UBaseToolAbility>()->GetVacancyAbilityDataPtr();
		if (!AbilityData || !AbilityData->IsValidAbilityData())
		{
			if (DebugTools())
			{
				UE_LOG(
					LogTemp,
					Warning,
					TEXT("%s contains an invalid tool ability data."),
					*GetNameSafe(this));
			}

			continue;
		}

		return TArray{ *AbilityData };
	}

	if (DebugTools())
	{
		UE_LOG(
		LogTemp,
		Warning,
		TEXT("%s does not have any valid tool ability data."),
		*GetNameSafe(this));
	}
	
	return TArray<FVacancyAbilityData>();
}

void ABaseTool::GrantToolAbilities(
	const AVacancyPlayerCharacter* ReceivingCharacter) const
{
	if (!IsValid(ReceivingCharacter))
	{
		if (DebugTools())
		{
			UE_LOG(
			LogTemp,
			Warning,
			TEXT("GrantToolAbilities failed for %s: character is invalid."),
			*GetNameSafe(this));
		}
		
		return;
	}

	UVacancyAbilitySystemComponent* AbilitySystem =
		ReceivingCharacter->GetVacancyAbilitySystemComponent();

	if (!IsValid(AbilitySystem))
	{
		if (DebugTools())
		{
			UE_LOG(
			LogTemp,
			Warning,
			TEXT("GrantToolAbilities failed for %s: ASC is invalid."),
			*GetNameSafe(this));
		}

		return;
	}

	for (const TSubclassOf<UBaseToolAbility>& AbilityClass
		: ToolData.ToolAbilityClasses)
	{
		if (!AbilityClass)
		{
			if (DebugTools())
			{
				UE_LOG(
				LogTemp,
				Warning,
				TEXT("GrantToolAbilities failed for %s: AbilityClass is null."),
				*GetNameSafe(this));
			}

			continue;
		}

		AbilitySystem->TryGiveAbilityByClass(AbilityClass);
	}
}

bool ABaseTool::DebugTools()
{
	return bDebugTools;
}