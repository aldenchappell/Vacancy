// Fill out your copyright notice in the Description page of Project Settings.

#include "Systems/Investigation/Objectives/BaseVacancyCaseObjective.h"
#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Components/Characters/Player/PlayerObjectiveComponent/PlayerObjectiveComponent.h"
#include "Systems/Investigation/Objectives/VacancyObjectiveActor.h"
#include "Utilities/Gameplay/VacancyPlayerUtils.h"

static bool GVacancyObjectiveLogging = false;

static FAutoConsoleVariableRef CVarEnableObjectiveLoggingRef(
	TEXT("Vacancy.Objective.EnableLogging"),
	GVacancyObjectiveLogging,
	TEXT("Enable logging for objectives."),
	ECVF_Cheat
);

void UBaseVacancyCaseObjective::OnObjectiveInitialized_Implementation()
{
	SortObjectivesByProgressionIndex();

	if (DebugObjectiveState())
	{
		UE_LOG(
			LogTemp,
			Log,
			TEXT("Objective initialized. ObjectiveID: %s"),
			*ObjectiveData.ObjectiveID.ToString()
		);
	}
}

void UBaseVacancyCaseObjective::InitializeObjective()
{
	OnObjectiveInitialized();
}

bool UBaseVacancyCaseObjective::IsObjectiveActive() const
{
	return !bIsObjectiveCompleted;
}

bool UBaseVacancyCaseObjective::IsObjectiveCompleted() const
{
	return bIsObjectiveCompleted;
}

FName UBaseVacancyCaseObjective::GetObjectiveID() const
{
	if (ObjectiveData.ObjectiveID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetObjectiveID: ObjectiveID is None for Objective."));
		return FName("InvalidObjectiveID");
	}
	
	return ObjectiveData.ObjectiveID;
}

void UBaseVacancyCaseObjective::SetObjectiveOwnerActor(AActor* NewOwnerActor)
{
	if (!IsValid(NewOwnerActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetObjectiveOwnerActor called with null NewOwnerActor."));
		return;
	}

	// Set the owner of this objective to the new actor
	ObjectiveOwnerActor = NewOwnerActor;
}

void UBaseVacancyCaseObjective::SetObjectiveStatus(
	const EVacancyCaseObjectiveStatus NewStatus,
	const AVacancyPlayerCharacter* PlayerCharacter)
{
	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetObjectiveStatus called with null PlayerCharacter."));
		return;
	}

	if (NewStatus == EVacancyCaseObjectiveStatus::MAX)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("SetObjectiveStatus failed: invalid NewStatus for ObjectiveID %s."),
			*ObjectiveData.ObjectiveID.ToString()
		);

		return;
	}

	if (ObjectiveStatus == NewStatus)
	{
		return;
	}

	ObjectiveStatus = NewStatus;
	bIsObjectiveCompleted = NewStatus == EVacancyCaseObjectiveStatus::Completed;

	HandleEnterObjectiveState(NewStatus, PlayerCharacter);

	OnObjectiveStatusChanged.Broadcast(NewStatus, PlayerCharacter);

	if (NewStatus == EVacancyCaseObjectiveStatus::Completed)
	{
		OnObjectiveCompleted.Broadcast();
	}
}

void UBaseVacancyCaseObjective::MarkObjectiveAsActive(const AVacancyPlayerCharacter* PlayerCharacter)
{
	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("MarkObjectiveAsActive called with null PlayerCharacter."));
		return;
	}

	SetObjectiveStatus(EVacancyCaseObjectiveStatus::Active, PlayerCharacter);
}

void UBaseVacancyCaseObjective::MarkObjectiveAsCompleted(const AVacancyPlayerCharacter* PlayerCharacter)
{
	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("MarkObjectiveAsCompleted called with null PlayerCharacter."));
		return;
	}

	SetObjectiveStatus(EVacancyCaseObjectiveStatus::Completed, PlayerCharacter);
}

void UBaseVacancyCaseObjective::MarkObjectiveAsFailed(const AVacancyPlayerCharacter* PlayerCharacter, const FString& FailReason)
{
	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("MarkObjectiveAsFailed called with null PlayerCharacter."));
		return;
	}

	SetObjectiveStatus(EVacancyCaseObjectiveStatus::Failed, PlayerCharacter);
}


bool UBaseVacancyCaseObjective::ShouldDisplayObjective() const
{
	//also, we're only going to display objectives that are NOT completed.
	return ObjectiveData.bIsObjectiveDisplayedInHUD && !bIsObjectiveCompleted;
}

bool UBaseVacancyCaseObjective::IsPreviousObjectiveCompleted(const int32 PreviousObjectiveIndex) const
{
	if (PreviousObjectiveIndex < 0 || PreviousObjectiveIndex >= ObjectiveData.Objectives.Num())
	{
		UE_LOG(LogTemp, Warning,
			TEXT("IsPreviousObjectiveCompleted: PreviousObjectiveIndex %d is out of bounds for Objective with ID %s."),
			PreviousObjectiveIndex, *ObjectiveData.ObjectiveID.ToString());
		return false;
	}

	const FVacancyCaseObjectiveStateData& PreviousObjective = ObjectiveData.Objectives[PreviousObjectiveIndex];
	return PreviousObjective.bIsOptional || bIsObjectiveCompleted;
}

FVacancyCaseObjectiveStateData UBaseVacancyCaseObjective::GetObjectiveAtIndex(const int32 ObjectiveIndex) const
{
	if (ObjectiveData.Objectives.Num() <= ObjectiveIndex)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("GetObjectiveAtIndex: ObjectiveIndex %d is out of bounds for Objective with ID %s."),
			ObjectiveIndex, *ObjectiveData.ObjectiveID.ToString());
		return FVacancyCaseObjectiveStateData();
	}

	return ObjectiveData.Objectives[ObjectiveIndex];
}

bool UBaseVacancyCaseObjective::SetObjectiveState(
	const FName& ObjectiveID,
	const EVacancyCaseObjectiveStatus NewState)
{
	if (ObjectiveID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("SetObjectiveState failed: ObjectiveID is None."));
		return false;
	}

	if (ObjectiveData.ObjectiveID != ObjectiveID)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("SetObjectiveState failed: ObjectiveID %s does not match this objective's ID %s."),
			*ObjectiveID.ToString(),
			*ObjectiveData.ObjectiveID.ToString()
		);

		return false;
	}

	if (NewState == EVacancyCaseObjectiveStatus::MAX)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetObjectiveState failed: invalid NewState."));
		return false;
	}

	ObjectiveStatus = NewState;
	bIsObjectiveCompleted = NewState == EVacancyCaseObjectiveStatus::Completed;

	if (NewState == EVacancyCaseObjectiveStatus::Completed)
	{
		OnObjectiveCompleted.Broadcast();
	}

	return true;
}

void UBaseVacancyCaseObjective::HandleEnterObjectiveState(
	const EVacancyCaseObjectiveStatus NewState,
	const AVacancyPlayerCharacter* PlayerCharacter)
{
	if (NewState == EVacancyCaseObjectiveStatus::MAX)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("HandleEnterObjectiveState failed: invalid NewState for ObjectiveID %s."),
			*ObjectiveData.ObjectiveID.ToString()
		);

		return;
	}

	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("HandleEnterObjectiveState failed: invalid PlayerCharacter for ObjectiveID %s."),
			*ObjectiveData.ObjectiveID.ToString()
		);

		return;
	}

	switch (NewState)
	{
		case EVacancyCaseObjectiveStatus::Active:
			HandleEnterActiveState(PlayerCharacter);
			break;

		case EVacancyCaseObjectiveStatus::Completed:
			HandleEnterCompletedState(PlayerCharacter);
			break;

		case EVacancyCaseObjectiveStatus::Failed:
			HandleEnterFailedState(PlayerCharacter);
			break;

		default:
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("HandleEnterObjectiveState: unhandled state %d for ObjectiveID %s."),
				static_cast<int32>(NewState),
				*ObjectiveData.ObjectiveID.ToString()
			);
			break;
	}
}

void UBaseVacancyCaseObjective::SortObjectivesByProgressionIndex() const
{
	if (ObjectiveData.Objectives.Num() <= 1)
	{
		return; // No need to sort if there's 0 or 1 objective
	}

	//sort the objectives by their index in the case file, so that they are displayed in the correct order to the player.
	TArray<FVacancyCaseObjectiveStateData> UnsortedObjectives = ObjectiveData.Objectives;
	ObjectiveData.SortObjectivesByIndex(UnsortedObjectives);
}

void UBaseVacancyCaseObjective::HandleEnterActiveState_Implementation(const AVacancyPlayerCharacter* PlayerCharacter)
{
	if (DebugObjectiveState())
	{
		UE_LOG(LogTemp, Log, TEXT("Objective is now active. ObjectiveID: %s"), *ObjectiveData.ObjectiveID.ToString());
	}
}

void UBaseVacancyCaseObjective::HandleEnterCompletedState_Implementation(const AVacancyPlayerCharacter* PlayerCharacter)
{
	if (DebugObjectiveState())
	{
		UE_LOG(LogTemp, Log, TEXT("Objective completed. ObjectiveID: %s"), *ObjectiveData.ObjectiveID.ToString());
	}

	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleEnterCompletedState called with null PlayerCharacter."));
		return;
	}

	if (UPlayerObjectiveComponent* PlayerObjectiveComponent = UVacancyPlayerUtils::GetPlayerComponent<UPlayerObjectiveComponent>(PlayerCharacter))
	{
		PlayerObjectiveComponent->ClearActiveObjective();
		if (ObjectiveOwnerActor)
		{
			if (AVacancyObjectiveActor* ObjectiveOwner = Cast<AVacancyObjectiveActor>(ObjectiveOwnerActor))
			{
				ObjectiveOwner->OnObjectiveCompleted(this, PlayerCharacter);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleEnterCompletedState: PlayerObjectiveComponent is null for PlayerCharacter %s."), *PlayerCharacter->GetName());
	}
}

void UBaseVacancyCaseObjective::HandleEnterFailedState_Implementation(const AVacancyPlayerCharacter* PlayerCharacter)
{
	if (DebugObjectiveState())
	{
		UE_LOG(LogTemp, Log, TEXT("Objective failed. ObjectiveID: %s"), *ObjectiveData.ObjectiveID.ToString());
	}
}

bool UBaseVacancyCaseObjective::DebugObjectiveState()
{
	return GVacancyObjectiveLogging;
}