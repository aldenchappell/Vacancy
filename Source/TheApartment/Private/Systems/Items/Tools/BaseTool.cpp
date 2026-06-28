// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Items/Tools/BaseTool.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "UI/VacancyHUDData.h"


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
		UE_LOG(LogTemp, Warning, TEXT("SetToolAttachmentStateInfo called with null AttachedTool."));
		return;
	}

	if (!IsValid(NewToolAttachmentStateInfo.ToolClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetToolAttachmentStateInfo called with null ToolClass."));
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
				UnequippingCharacter->UpdateHUDByType(EVacancyHUDType::ToolHUD);
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