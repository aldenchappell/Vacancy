// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Characters/Player/Interaction/PlayerInteractionComponent.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Characters/Player/VacancyPlayerController.h"
#include "Components/Characters/Player/Interaction/InteractableInterface.h"
#include "GameFramework/Character.h"
#include "Systems/Interaction/Interactions/VacancyInteractionBase.h"
#include "UI/VacancyHUD.h"
#include "UI/Interaction/InteractionWidget.h"


UPlayerInteractionComponent::UPlayerInteractionComponent()
{

}

void UPlayerInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AVacancyPlayerCharacter* PlayerChar = Cast<AVacancyPlayerCharacter>(GetOwner()))
	{
		PlayerCharacter = PlayerChar; // Cache the player character reference for later use
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent is attached to an actor that is not a VacancyPlayerCharacter."));
	}

	
}

void UPlayerInteractionComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TimeSinceLastScan >= InteractionScanInfo.TimeBetweenScans)
	{
		ScanForInteractables();
		TimeSinceLastScan = 0.f;
	}
	else
	{
		TimeSinceLastScan += DeltaTime;
	}
}

void UPlayerInteractionComponent::StartInteraction(const FInteractionInfo& InteractionInfo) const
{
	if (!InteractionInfo.InteractableActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartInteraction called with null InteractableActor in InteractionInfo."));
		return; // If the interactable actor is null, exit early
	}

	if (!InteractionInfo.InteractableActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("StartInteraction called with an actor that does not implement the InteractableInterface: %s"),
			*InteractionInfo.InteractableActor->GetName());
		return; // If the interactable actor does not implement the interface, exit early
	}

	// Call the StartInteraction function on the interactable actor
	IInteractableInterface::Execute_StartInteraction(InteractionInfo.InteractableActor, PlayerCharacter);

	// Update the HUD with the interaction visual info
	if (const AVacancyPlayerController* PlayerController = Cast<AVacancyPlayerController>(PlayerCharacter->GetController()))
	{
		if (const AVacancyHUD* VacancyHUD = PlayerController->GetVacancyHUD())
		{
			if (UInteractionWidget* InteractionWidget = VacancyHUD->GetInteractionWidget())
			{				const FInteractionVisualInfo VisualInfo = IInteractableInterface::Execute_GetInteractionVisualInfo(InteractionInfo.InteractableActor);
				
				InteractionWidget->DisplayNewInteraction(VisualInfo); // Update the interaction widget with the visual info from the interactable actor
			}
		}
	}
}

void UPlayerInteractionComponent::EndInteraction(const FInteractionInfo& InteractionInfo) const
{
	if (!InteractionInfo.InteractableActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartInteraction called with null InteractableActor in InteractionInfo."));
		return; // If the interactable actor is null, exit early
	}

	if (!InteractionInfo.InteractableActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("StartInteraction called with an actor that does not implement the InteractableInterface: %s"),
			*InteractionInfo.InteractableActor->GetName());
		return; // If the interactable actor does not implement the interface, exit early
	}

	IInteractableInterface::Execute_EndInteraction(InteractionInfo.InteractableActor, PlayerCharacter); // Call the EndInteraction function on the interactable actor
}

void UPlayerInteractionComponent::ScanForInteractables()
{
	const float MaxScanRange = InteractionScanInfo.MaxInteractionScanDistance;
	if (MaxScanRange <= 0.f)
	{
		return; // If the max scan range is zero or negative, exit early
	}

	AVacancyPlayerCharacter* PlayerCharacterPtr = Cast<AVacancyPlayerCharacter>(GetOwner());
	if (!PlayerCharacterPtr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent is attached to an actor that is not a VacancyPlayerCharacter."));
		return; // If the owner is not a character, exit early
	}

	//Get Eyes Viewpoint
	FVector PlayerEyesLoc = FVector();
	FRotator PlayerEyesRot = FRotator();
	PlayerCharacterPtr->GetActorEyesViewPoint(PlayerEyesLoc, PlayerEyesRot);

	//Calculate Trace End
	const FVector TraceStart = PlayerEyesLoc;
	const FVector TraceEnd = TraceStart + (PlayerEyesRot.Vector() * MaxScanRange);

	TArray<FHitResult> HitResults;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	const UWorld* World = GetWorld();
	if (!World)
	{
		return; // If the world context is not valid, exit early
	}

	World->LineTraceMultiByChannel(HitResults, TraceStart, TraceEnd, InteractionScanInfo.TraceChannel, QueryParams);
	if (HitResults.Num() == 0)
	{
		LastActiveInteractable = nullptr; // If no hits, clear the last active interactable
		return;
	}

	for (const FHitResult& HitResult : HitResults)
	{
		AActor* PotentialInteractable = ActorToInteractable(HitResult.GetActor());
		if (PotentialInteractable)
		{
			LastActiveInteractable = PotentialInteractable; // Set the last active interactable to the first valid hit
			return;
		}

		const FInteractionValidationData ValidationData = PotentialInteractable->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass())
			? IInteractableInterface::Execute_GetInteraction(PotentialInteractable)->GetValidationData()
			: FInteractionValidationData();

		const float DistanceToInteractable = FVector::Dist(PotentialInteractable->GetActorLocation(), PlayerCharacterPtr->GetActorLocation());
		if (DistanceToInteractable > ValidationData.MaxInteractionDistance)
		{
			continue; // If the potential interactable is out of range, skip to the next hit
		}

		const bool bIsInteractionValid = PotentialInteractable->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass())
			? IInteractableInterface::Execute_GetInteraction(PotentialInteractable)->CanInteract(PlayerCharacterPtr)
			: false;
		if (!bIsInteractionValid)
		{
			continue; // If the potential interactable is not valid for interaction, skip to the next hit
		}

		// Start interaction with the first valid interactable hit
		StartInteraction(FInteractionInfo(IInteractableInterface::Execute_GetInteraction(PotentialInteractable)->GetInteractionInfo()));

		LastActiveInteractable = PotentialInteractable; // Set the last active interactable to the first valid hit
		return;
	}
}

AActor* UPlayerInteractionComponent::ActorToInteractable(AActor* InActor)
{
	if (!InActor)
	{
		return nullptr;
	}

	// Check if the actor implements the interactable interface
	if (InActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		return InActor; // If it does, return the actor as a valid interactable
	}
	
	return nullptr;
}

bool UPlayerInteractionComponent::TryInteractWith(AActor* InInteractableActor)
{
	if (!InInteractableActor)	
	{
		return false; // If the input actor is null, return false
	}

	if (!InInteractableActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		return false; // If the input actor does not implement the interactable interface, return false
	}

	IInteractableInterface::Execute_Interact(InInteractableActor, PlayerCharacter);
	return true; // If the interaction was attempted, return true
}