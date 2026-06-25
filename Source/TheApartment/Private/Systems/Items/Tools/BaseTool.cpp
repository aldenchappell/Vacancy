// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Items/Tools/BaseTool.h"


ABaseTool::ABaseTool()
{
	ToolMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Tool Mesh"));
	ToolMesh->SetupAttachment(RootComponent);

	ToolMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ToolMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
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