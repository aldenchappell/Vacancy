// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Items/Tools/BaseTool.h"


ABaseTool::ABaseTool()
{
	ToolMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Tool Mesh"));
	SetRootComponent(ToolMesh);

	ToolMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ToolMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	//enable physics simulation for the tool mesh
	ToolMesh->SetSimulatePhysics(true);
}

void ABaseTool::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseTool::OnToolEquipped_Implementation(AVacancyPlayerCharacter* UnequippingCharacter)
{
	
}

void ABaseTool::OnToolUnequipped_Implementation(AVacancyPlayerCharacter* UnequippingCharacter)
{
	
}

FName ABaseTool::GetToolAttachSocket() const
{
	return ToolAttachmentStateInfo.ToolAttachSocket;
}