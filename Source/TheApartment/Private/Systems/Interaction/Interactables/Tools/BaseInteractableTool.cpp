// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Interaction/Interactables/Tools/BaseInteractableTool.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Systems/Interaction/Interactions/Tools/BaseToolPickupInteraction.h"

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

void ABaseInteractableTool::Interact_Implementation(AVacancyPlayerCharacter* InteractingCharacter)
{
	if (!IsValid(InteractingCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("Interact called with null InteractingCharacter on %s"), *GetName());
		return; // If the interacting character is null, do nothing.
	}
	
	Super::Interact_Implementation(InteractingCharacter);
	
	if (UBaseToolPickupInteraction* Interaction = Cast<UBaseToolPickupInteraction>(Execute_GetInteraction(this)))
	{
		//Interaction->ToolAttachmentStateInfo.AttachedTool = SpawnedTool;
	}
}


