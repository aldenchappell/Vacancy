// Fill out your copyright notice in the Description page of Project Settings.

#include "Systems/Investigation/Objectives/BaseVacancyCaseObjective.h"
#include "Characters/Player/VacancyPlayerCharacter.h"

static TAutoConsoleVariable<bool> CVarEnableObjectiveLogging(
	TEXT("Vacancy.Objective.EnableLogging"),
	false,
	TEXT("Enable logging for objectives."),
	ECVF_Cheat
);

void UBaseVacancyCaseObjective::OnObjectiveInitialized_Implementation()
{
	OnObjectiveStatusChanged.AddDynamic(this, &UBaseVacancyCaseObjective::HandleEnterObjectiveState);
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

void UBaseVacancyCaseObjective::SetObjectiveStatus(const EVacancyCaseObjectiveStatus NewStatus, const AVacancyPlayerCharacter* PlayerCharacter)
{
	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetObjectiveStatus called with null PlayerCharacter."));
		return;
	}
	
	if (NewStatus == EVacancyCaseObjectiveStatus::MAX)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetObjectiveStatus: Invalid NewStatus for Objective: %s."), *ObjectiveData.ObjectiveID.ToString());
		return;
	}

	ObjectiveStatus = NewStatus;
	OnObjectiveStatusChanged.Broadcast(NewStatus, nullptr);
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

bool UBaseVacancyCaseObjective::SetObjectiveState(const FName& ObjectiveID, const EVacancyCaseObjectiveStatus NewState)
{
	for (FVacancyCaseObjectiveStateData& ObjectiveState : ObjectiveData.Objectives)
	{
		if (ObjectiveData.ObjectiveID == ObjectiveID)
		{
			ObjectiveStatus = NewState;
			bIsObjectiveCompleted = (NewState == EVacancyCaseObjectiveStatus::Completed);
			OnObjectiveCompleted.Broadcast();
			return true;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("SetObjectiveState: Objective with ID %s not found."), *ObjectiveID.ToString());
	return false;
}

void UBaseVacancyCaseObjective::HandleEnterObjectiveState(const EVacancyCaseObjectiveStatus NewState,
const AVacancyPlayerCharacter* PlayerCharacter)
{
	if (NewState == EVacancyCaseObjectiveStatus::MAX)
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleEnterObjectiveState: Invalid NewState for Objective: %s."), *ObjectiveData.ObjectiveID.ToString());
		return;
	}

	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleEnterObjectiveState: Invalid PlayerCharacter for ObjectiveID %s."), *ObjectiveData.ObjectiveID.ToString());
		return;
	}

	switch (NewState)
	{
		case EVacancyCaseObjectiveStatus::Active:
			HandleEnterActiveState();
			break;
		case EVacancyCaseObjectiveStatus::Completed:
			HandleEnterCompletedState();
			break;
		case EVacancyCaseObjectiveStatus::Failed:
			HandleEnterFailedState();
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("HandleEnterObjectiveState: Unhandled state %d for ObjectiveID %s."), static_cast<int32>(NewState), *ObjectiveData.ObjectiveID.ToString());
			break;
	}

	OnObjectiveStatusChanged.Broadcast(NewState, PlayerCharacter);
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

void UBaseVacancyCaseObjective::HandleEnterActiveState_Implementation()
{
	if (DebugObjectiveState())
	{
		UE_LOG(LogTemp, Log, TEXT("Objective is now active. ObjectiveID: %s"), *ObjectiveData.ObjectiveID.ToString());
	}
}

void UBaseVacancyCaseObjective::HandleEnterCompletedState_Implementation()
{
	if (DebugObjectiveState())
	{
		UE_LOG(LogTemp, Log, TEXT("Objective completed. ObjectiveID: %s"), *ObjectiveData.ObjectiveID.ToString());
	}
}

void UBaseVacancyCaseObjective::HandleEnterFailedState_Implementation()
{
	if (DebugObjectiveState())
	{
		UE_LOG(LogTemp, Log, TEXT("Objective failed. ObjectiveID: %s"), *ObjectiveData.ObjectiveID.ToString());
	}
}

bool UBaseVacancyCaseObjective::DebugObjectiveState()
{
	return CVarEnableObjectiveLogging.GetValueOnGameThread();
}