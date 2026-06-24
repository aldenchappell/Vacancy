// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Characters/Player/Interaction/PlayerInteractionComponent.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Characters/Player/VacancyPlayerController.h"
#include "Components/Characters/Player/Interaction/InteractableInterface.h"
#include "Systems/Interaction/Interactions/VacancyInteractionBase.h"
#include "UI/VacancyHUD.h"
#include "UI/Interaction/InteractionWidget.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "Engine/World.h"

static TAutoConsoleVariable<bool> CVarEnableInteractableLogging(
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
	if (!InteractionInfo.InteractionBasicInfo.InteractableActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryUseInteraction called with null InteractableActor in InteractionInfo."));
		return false; // If the interactable actor is null, exit early
	}

	if (!InteractionInfo.InteractionBasicInfo.InteractableActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("TryUseInteraction called with an actor that does not implement the InteractableInterface: %s"),
			*InteractionInfo.InteractionBasicInfo.InteractableActor->GetName());
		return false; // If the interactable actor does not implement the interface, exit early
	}

	// Call the Interact function on the interactable actor
	IInteractableInterface::Execute_Interact(InteractionInfo.InteractionBasicInfo.InteractableActor, PlayerCharacter); 
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

	CreateInteractionWidgetComp();
}

void UPlayerInteractionComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!CanExecuteInteractions())
	{
		return;
	}
	
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
		// If the interactable actor is null, exit early
		return; 
	}

	if (!InteractionInfo.InteractionBasicInfo.InteractableActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("StartInteraction called with an actor that does not implement the InteractableInterface: %s"),
			*InteractionInfo.InteractionBasicInfo.InteractableActor->GetName());
		// If the interactable actor does not implement the interface, exit early
		return; 
	}

	// Call the StartInteraction function on the interactable actor
	IInteractableInterface::Execute_StartInteraction(InteractionInfo.InteractionBasicInfo.InteractableActor, PlayerCharacter);

	// Update the HUD with the interaction visual info
	if (const AVacancyPlayerController* PlayerController = Cast<AVacancyPlayerController>(PlayerCharacter->GetController()))
	{
		if (const AVacancyHUD* VacancyHUD = PlayerController->GetVacancyHUD())
		{
			
			if (InteractionWidgetInstance)
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
					UpdateInteractionWidgetLoc(InteractableActor);
					// Update the interaction widget with the visual info from the interactable actor
					InteractionWidgetInstance->DisplayNewInteraction(InteractionInfo); 
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

	if (const AVacancyPlayerController* PlayerController = Cast<AVacancyPlayerController>(PlayerCharacter->GetController()))
	{
		if (const AVacancyHUD* VacancyHUD = PlayerController->GetVacancyHUD())
		{
			
			if (InteractionWidgetInstance)
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
					
					//Clear interaction display on the widget
					InteractionWidgetInstance->ClearInteractionDisplay();
				}
			}
		}
	}
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
			if (CVarEnableInteractableLogging.GetValueOnGameThread())
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
			if (CVarEnableInteractableLogging.GetValueOnGameThread())
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

		if (CVarEnableInteractableLogging.GetValueOnGameThread())
		{
			UE_LOG(
				LogTemp,
				Log,
				TEXT("PlayerInteractionComponent: New active interactable: %s"),
				*GetNameSafe(NewActiveInteractable)
			);
		}
	}
	else
	{
		if (CVarEnableInteractableLogging.GetValueOnGameThread())
		{
			UE_LOG(
				LogTemp,
				Log,
				TEXT("PlayerInteractionComponent: No active interactable found.")
			);
		}
		EndInteraction(NewInteractionInfo);
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
			const FGameplayTag InteractionIdentifier = InteractionInfo.InteractionVisualInfo.InteractionTag;
			if (const FName InteractionName = InteractionIdentifier.IsValid() ?
				InteractionIdentifier.GetTagName() : FName("InvalidInteraction");
				IgnoredInteractionIdentifiers.Contains(InteractionName))
			{
				// Skip this interaction if it's in the ignored list
				continue; 
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
				if (CVarEnableInteractableLogging.GetValueOnGameThread())
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

void UPlayerInteractionComponent::CreateInteractionWidgetComp()
{
	if (!IsValid(InteractionWidgetClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent: InteractionWidgetClass is not set."));
		return;
	}

	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent: PlayerCharacter is not valid."));
		return;
	}

	if (UWidgetComponent* InteractionWidgetComponent = NewObject<UWidgetComponent>(PlayerCharacter))
	{
		InteractionWidgetCompInstance = InteractionWidgetComponent;
		PlayerCharacter->AddInstanceComponent(InteractionWidgetComponent);
		InteractionWidgetComponent->RegisterComponent();

		InteractionWidgetInstance = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		if (!InteractionWidgetInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent: Failed to create InteractionWidgetInstance."));
			return;
		}

		InteractionWidgetComponent->SetWidget(InteractionWidgetInstance);
		InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		InteractionWidgetComponent->SetDrawSize(FVector2D(250.f, 50.f));
	}
}

void UPlayerInteractionComponent::ToggleInteractionWidgetCompVis(const bool bVisible) const
{
	if (!InteractionWidgetInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent: InteractionWidgetInstance is null."));
		return;
	}

	InteractionWidgetInstance->SwitchInteractionVisibility(bVisible);
}

void UPlayerInteractionComponent::UpdateInteractionWidgetLoc(const AActor* InteractableActor) const
{
	if (!IsValid(InteractableActor))
	{
		//if we don't have a valid interactable actor, hide the interaction display and clear its location out
		InteractionWidgetCompInstance->SetWorldLocation(FVector::ZeroVector);
		InteractionWidgetInstance->ClearInteractionDisplay();
		UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent: InteractableActor is not valid."));
		return;
	}

	if (!IsValid(InteractionWidgetInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent: InteractionWidgetInstance is not valid."));
		return;
	}

	if (!IsValid(InteractionWidgetCompInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent: InteractionWidgetCompInstance is not valid."));
		return;
	}

	const FVector InteractionPos =
		IInteractableInterface::Execute_GetInteractionTransformComponent(InteractableActor)->GetComponentLocation();

	InteractionWidgetCompInstance->SetWorldLocation(InteractionPos);
}

void UPlayerInteractionComponent::EnterHide() const
{
	// Use the GetInteractionFromIdentifier utility function to find the interaction with the "EnterHide" identifier
}

void UPlayerInteractionComponent::ExitHide() const
{
	// Use the GetInteractionFromIdentifier utility function to find the interaction with the "ExitHide" identifier
}

bool UPlayerInteractionComponent::CanExecuteInteractions() const
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent: PlayerCharacter is not valid."));
		return false;
	}

	return !ActiveInteractionTags.HasAny(InteractionBlockingTags);
}

void UPlayerInteractionComponent::ApplyActiveInteractionTags(const FGameplayTagContainer& TagsToApply)
{
	if (TagsToApply.IsEmpty())
	{
		return;
	}

	for (const FGameplayTag& Tag : TagsToApply)
	{
		if (!Tag.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent: Invalid tag in TagsToApply."));
			return;
		}

		if (ActiveInteractionTags.HasTag(Tag))
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent: Tag %s is already active."), *Tag.ToString());
			return;
		}
	}

	ActiveInteractionTags.AppendTags(TagsToApply);
}

void UPlayerInteractionComponent::RemoveActiveInteractionTags(const FGameplayTagContainer& TagsToRemove)
{
	if (TagsToRemove.IsEmpty())
	{
		return;
	}

	for (const FGameplayTag& Tag : TagsToRemove)
	{
		if (!Tag.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent: Invalid tag in TagsToRemove."));
			return;
		}

		if (!ActiveInteractionTags.HasTag(Tag))
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerInteractionComponent: Tag %s is not active."), *Tag.ToString());
			return;
		}
	}

	for (const FGameplayTag& Tag : TagsToRemove)
	{
		ActiveInteractionTags.RemoveTag(Tag);
	}
}