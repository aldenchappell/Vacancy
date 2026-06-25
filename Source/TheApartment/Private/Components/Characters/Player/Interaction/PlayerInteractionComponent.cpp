// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Characters/Player/Interaction/PlayerInteractionComponent.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Characters/Player/VacancyPlayerController.h"
#include "Components/Characters/Player/Interaction/InteractableInterface.h"
#include "GameFramework/Character.h"
#include "Systems/Interaction/Interactions/VacancyInteractionBase.h"
#include "UI/VacancyHUD.h"
#include "UI/Interaction/InteractionWidget.h"
//
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
//

static TAutoConsoleVariable<bool> CVarEnableInteractionActorLogging(
	TEXT("Vacancy.Interaction.EnableLogging"),
	false,
	TEXT("Enable logging for player interaction component."),
	ECVF_Cheat);

static TAutoConsoleVariable<bool> CVarEnableInteractionScanLogging(
	TEXT("Vacancy.Interaction.Scan.EnableLogging"),
	false,
	TEXT("Enable logging for player interaction component scan."),
	ECVF_Cheat);

UPlayerInteractionComponent::UPlayerInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UPlayerInteractionComponent::TryInteractWithActiveInteractable() const
{
	if (!IsValid(LastActiveInteractable))
	{
		UE_LOG(LogTemp, Warning, TEXT("TryInteractWith called with null LastActiveInteractable."));
		return false; // If the interactable actor is null, exit early
	}

	if (!LastActiveInteractable->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("TryInteractWith called with an actor that does not implement the InteractableInterface: %s"),
			*LastActiveInteractable->GetName());
		return false; // If the interactable actor does not implement the interface, exit early
	}

	IInteractableInterface::Execute_Interact(LastActiveInteractable, PlayerCharacter); // Call the Interact function on the interactable actor

	return true;
}

bool UPlayerInteractionComponent::TryUseInteraction(const FInteractionInfo& InteractionInfo) const
{
	AActor* InteractableActor = InteractionInfo.InteractionBasicInfo.InteractableActor;

	if (!IsValid(InteractableActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("TryUseInteraction called with null InteractableActor in InteractionInfo."));
		return false;
	}

	if (!InteractableActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("TryUseInteraction called with an actor that does not implement InteractableInterface: %s"),
			*GetNameSafe(InteractableActor)
		);

		return false;
	}

	IInteractableInterface::Execute_Interact(InteractableActor, PlayerCharacter);

	return true;
}

void UPlayerInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AVacancyPlayerCharacter>(GetOwner());

	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent is attached to an actor that is not a VacancyPlayerCharacter."));
		return;
	}

	UE_LOG(
		LogTemp,
		Log,
		TEXT("PlayerInteractionComponent initialized on %s"),
		*GetNameSafe(PlayerCharacter)
	);
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
	if (!InteractionInfo.InteractionBasicInfo.InteractableActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartInteraction called with null InteractableActor in InteractionInfo."));
		return; // If the interactable actor is null, exit early
	}

	if (!InteractionInfo.InteractionBasicInfo.InteractableActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("StartInteraction called with an actor that does not implement the InteractableInterface: %s"),
			*InteractionInfo.InteractionBasicInfo.InteractableActor->GetName());
		return; // If the interactable actor does not implement the interface, exit early
	}

	// Call the StartInteraction function on the interactable actor
	IInteractableInterface::Execute_StartInteraction(InteractionInfo.InteractionBasicInfo.InteractableActor, PlayerCharacter);

	// Update the HUD with the interaction visual info
	if (const AVacancyPlayerController* PlayerController = Cast<AVacancyPlayerController>(PlayerCharacter->GetController()))
	{
		if (const AVacancyHUD* VacancyHUD = PlayerController->GetVacancyHUD())
		{
			
			if (UInteractionWidget* InteractionWidget = VacancyHUD->GetInteractionWidget())
			{
				const AActor* InteractableActor = InteractionInfo.InteractionBasicInfo.InteractableActor;
				if (!InteractableActor)
				{
					UE_LOG(LogTemp, Warning, TEXT("StartInteraction: InteractableActor is null in InteractionInfo."));
					return; // If the interactable actor is null, exit early
				}
				const FInteractionInfo InteractableInteractionInfo =
					IInteractableInterface::Execute_GetInteractionInfo(InteractableActor);
				{
					InteractionWidget->DisplayNewInteraction(InteractionInfo); // Update the interaction widget with the visual info from the interactable actor
				}
			}
		}
	}
}

void UPlayerInteractionComponent::EndInteraction(const FInteractionInfo& InteractionInfo) const
{
	if (!InteractionInfo.InteractionBasicInfo.InteractableActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartInteraction called with null InteractableActor in InteractionInfo."));
		return; // If the interactable actor is null, exit early
	}

	if (!InteractionInfo.InteractionBasicInfo.InteractableActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("StartInteraction called with an actor that does not implement the InteractableInterface: %s"),
			*InteractionInfo.InteractionBasicInfo.InteractableActor->GetName());
		return; // If the interactable actor does not implement the interface, exit early
	}

	// Call the EndInteraction function on the interactable actor
	IInteractableInterface::Execute_EndInteraction(InteractionInfo.InteractionBasicInfo.InteractableActor, PlayerCharacter); 
}

void UPlayerInteractionComponent::ScanForInteractables()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent: No valid world."));
		return;
	}

	AVacancyPlayerCharacter* PlayerCharacterPtr = PlayerCharacter.Get();

	if (!IsValid(PlayerCharacterPtr))
	{
		PlayerCharacterPtr = Cast<AVacancyPlayerCharacter>(GetOwner());
		PlayerCharacter = PlayerCharacterPtr;
	}

	if (!IsValid(PlayerCharacterPtr))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent: Owner is not AVacancyPlayerCharacter."));
		return;
	}

	const float MaxScanRange = InteractionScanInfo.MaxInteractionScanDistance;
	if (MaxScanRange <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent: MaxInteractionScanDistance is <= 0."));
		return;
	}

	FVector PlayerEyesLoc;
	FRotator PlayerEyesRot;
	PlayerCharacterPtr->GetActorEyesViewPoint(PlayerEyesLoc, PlayerEyesRot);

	const FVector TraceStart = PlayerEyesLoc;
	const FVector TraceEnd = TraceStart + PlayerEyesRot.Vector() * MaxScanRange;

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(PlayerInteractionScan), false);
	QueryParams.AddIgnoredActor(GetOwner());

	TArray<FHitResult> HitResults;

	const bool bHit = World->LineTraceMultiByChannel(
		HitResults,
		TraceStart,
		TraceEnd,
		InteractionScanInfo.TraceChannel,
		QueryParams
	);

	const bool bDebugScan = CVarEnableInteractionScanLogging.GetValueOnGameThread();

	if (bDebugScan)
	{
		DrawDebugLine(
			World,
			TraceStart,
			TraceEnd,
			bHit ? FColor::Green : FColor::Red,
			false,
			InteractionScanInfo.TimeBetweenScans,
			0,
			2.f
		);

		UE_LOG(
			LogTemp,
			Log,
			TEXT("PlayerInteractionComponent: Scan ran. Hit count: %d. Start: %s End: %s"),
			HitResults.Num(),
			*TraceStart.ToString(),
			*TraceEnd.ToString()
		);
	}

	AActor* NewActiveInteractable = nullptr;
	FInteractionInfo NewInteractionInfo;

	for (const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();

		if (bDebugScan)
		{
			UE_LOG(
				LogTemp,
				Log,
				TEXT("PlayerInteractionComponent: Hit actor: %s"),
				*GetNameSafe(HitActor)
			);
		}

		AActor* PotentialInteractable = ActorToInteractable(HitActor);
		if (!IsValid(PotentialInteractable))
		{
			continue;
		}

		UVacancyInteractionBase* Interaction = IInteractableInterface::Execute_GetInteraction(PotentialInteractable);
		if (!IsValid(Interaction))
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("PlayerInteractionComponent: %s implements InteractableInterface but returned null interaction."),
				*GetNameSafe(PotentialInteractable)
			);

			continue;
		}

		const FInteractionValidationData ValidationData = Interaction->GetValidationData();

		const float DistanceToInteractable = FVector::Dist(
			PlayerCharacterPtr->GetActorLocation(),
			HitResult.ImpactPoint
		);

		if (ValidationData.MaxInteractionDistance > 0.f &&
			DistanceToInteractable > ValidationData.MaxInteractionDistance)
		{
			if (CVarEnableInteractionActorLogging.GetValueOnGameThread())
			{
				UE_LOG(
					LogTemp,
					Log,
					TEXT("PlayerInteractionComponent: %s is out of range. Distance: %.2f Max: %.2f"),
					*GetNameSafe(PotentialInteractable),
					DistanceToInteractable,
					ValidationData.MaxInteractionDistance
				);
			}

			continue;
		}

		if (!Interaction->CanInteract(PlayerCharacterPtr))
		{
			if (CVarEnableInteractionActorLogging.GetValueOnGameThread())
			{
				UE_LOG(
					LogTemp,
					Log,
					TEXT("PlayerInteractionComponent: %s failed CanInteract."),
					*GetNameSafe(PotentialInteractable)
				);
			}

			continue;
		}

		NewActiveInteractable = PotentialInteractable;
		NewInteractionInfo = Interaction->GetInteractionInfo();

		break;
	}

	if (NewActiveInteractable == LastActiveInteractable)
	{
		return;
	}

	if (IsValid(LastActiveInteractable) &&
		LastActiveInteractable->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		if (UVacancyInteractionBase* OldInteraction = IInteractableInterface::Execute_GetInteraction(LastActiveInteractable))
		{
			EndInteraction(OldInteraction->GetInteractionInfo());
		}
	}

	LastActiveInteractable = NewActiveInteractable;

	if (IsValid(NewActiveInteractable))
	{
		StartInteraction(NewInteractionInfo);

		if (CVarEnableInteractionActorLogging.GetValueOnGameThread())
		{
			UE_LOG(
				LogTemp,
				Log,
				TEXT("PlayerInteractionComponent: New active interactable: %s"),
				*GetNameSafe(NewActiveInteractable)
			);
		}
	}
}

void UPlayerInteractionComponent::InitializeDefaultInteractions(const TArray<FName>& IgnoredInteractionIdentifiers)
{
	if (DefaultInteractions.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent: No default granted interactions set."));
		return;
	}

	for (const FDefaultInteraction& DefaultInteraction : DefaultInteractions)
	{
		for (const FInteractionInfo& InteractionInfo : DefaultInteraction.DefaultInteractionInfos)
		{
			if (IgnoredInteractionIdentifiers.Contains(InteractionInfo.InteractionBasicInfo.InteractionIdentifier))
			{
				continue; // Skip this interaction if it's in the ignored list
			}

			if (!TryUseInteraction(InteractionInfo))
			{
				UE_LOG(
					LogTemp,
					Warning,
					TEXT("PlayerInteractionComponent: Failed to initialize default interaction: %s"),
					*GetNameSafe(InteractionInfo.InteractionBasicInfo.InteractableActor)
				);
			}
			else
			{
				if (CVarEnableInteractionActorLogging.GetValueOnGameThread())
				{
					UE_LOG(
						LogTemp,
						Log,
						TEXT("PlayerInteractionComponent: Initialized default interaction: %s"),
						*GetNameSafe(InteractionInfo.InteractionBasicInfo.InteractableActor)
					);
				}
			}
		}
	}
}

AActor* UPlayerInteractionComponent::ActorToInteractable(AActor* InActor)
{
	if (!IsValid(InActor))
	{
		return nullptr;
	}

	if (InActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		return InActor;
	}

	return nullptr;
}

void UPlayerInteractionComponent::EnterHide() const
{
	
}

void UPlayerInteractionComponent::ExitHide() const
{
	
} 