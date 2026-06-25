// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Interaction/Interactables/Tools/BaseInteractableTool.h"

#include "Components/BoxComponent.h"

ABaseInteractableTool::ABaseInteractableTool()
{
	ToolMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tool Mesh"));
	SetRootComponent(ToolMesh);

	InteractionCollision->SetupAttachment(ToolMesh);
	InteractionTransformComponent->SetupAttachment(ToolMesh);
}

void ABaseInteractableTool::BeginPlay()
{
	Super::BeginPlay();
	
}


