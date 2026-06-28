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
	OnObjectiveStateChanged.AddDynamic(this, &UBaseVacancyCaseObjective::HandleEnterObjectiveState);
}

void UBaseVacancyCaseObjective::InitializeObjective()
{
	OnObjectiveInitialized();
}

bool UBaseVacancyCaseObjective::IsObjectiveActive(const int32 ObjectiveIndex) const
{
	return !ObjectiveData.Objectives[ObjectiveIndex].bIsObjectiveCompleted;
}

FName UBaseVacancyCaseObjective::GetObjectiveID(const UBaseVacancyCaseObjective*& Objective, const int32 ObjectiveIndex)
{
	if (!IsValid(Objective))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetObjectiveID called with null Objective."));
		return NAME_None;
	}

	if (Objective->GetObjectives().Num() <= ObjectiveIndex)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("GetObjectiveID: ObjectiveIndex %d is out of bounds for Objective with ID %s."),
			ObjectiveIndex, *Objective->GetObjectives()[0].ObjectiveID.ToString());
		return NAME_None;
	}
	
	return Objective->GetObjectives()[ObjectiveIndex].ObjectiveID;
}

bool UBaseVacancyCaseObjective::SetObjectiveState(const FName& ObjectiveID, const EVacancyCaseObjectiveStatus NewState)
{
	for (FVacancyCaseObjectiveStateData& ObjectiveState : ObjectiveData.Objectives)
	{
		if (ObjectiveState.ObjectiveID == ObjectiveID)
		{
			ObjectiveState.ObjectiveStatus = NewState;
			ObjectiveState.bIsObjectiveCompleted = (NewState == EVacancyCaseObjectiveStatus::Completed);
			return true;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("SetObjectiveState: Objective with ID %s not found."), *ObjectiveID.ToString());
	return false;
}

void UBaseVacancyCaseObjective::HandleEnterObjectiveState(EVacancyCaseObjectiveStatus NewState,
	const FName& ObjectiveID, const AVacancyPlayerCharacter* PlayerCharacter)
{
	if (NewState == EVacancyCaseObjectiveStatus::MAX)
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleEnterObjectiveState: Invalid state 'MAX' for ObjectiveID %s."), *ObjectiveID.ToString());
		return;
	}

	if (!ObjectiveID.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleEnterObjectiveState: Invalid ObjectiveID for state change to %d."), static_cast<int32>(NewState));
		return;
	}

	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleEnterObjectiveState: Invalid PlayerCharacter for ObjectiveID %s."), *ObjectiveID.ToString());
		return;
	}

	switch (NewState)
	{
		case EVacancyCaseObjectiveStatus::InProgress:
			HandleEnterActiveState();
			break;
		case EVacancyCaseObjectiveStatus::Completed:
			HandleEnterCompletedState();
			break;
		case EVacancyCaseObjectiveStatus::Failed:
			HandleEnterFailedState();
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("HandleEnterObjectiveState: Unhandled state %d for ObjectiveID %s."), static_cast<int32>(NewState), *ObjectiveID.ToString());
			break;
	}

	OnObjectiveStateChanged.Broadcast(NewState, ObjectiveID, PlayerCharacter);
}

void UBaseVacancyCaseObjective::HandleEnterActiveState_Implementation()
{
	if (DebugObjectiveState())
	{
		UE_LOG(LogTemp, Log, TEXT("Objective is now active. ObjectiveID: %s"), *ObjectiveData.Objectives[0].ObjectiveID.ToString());
	}
}

void UBaseVacancyCaseObjective::HandleEnterCompletedState_Implementation()
{
	if (DebugObjectiveState())
	{
		UE_LOG(LogTemp, Log, TEXT("Objective completed. ObjectiveID: %s"), *ObjectiveData.Objectives[0].ObjectiveID.ToString());
	}
}

void UBaseVacancyCaseObjective::HandleEnterFailedState_Implementation()
{
	if (DebugObjectiveState())
	{
		UE_LOG(LogTemp, Log, TEXT("Objective failed. ObjectiveID: %s"), *ObjectiveData.Objectives[0].ObjectiveID.ToString());
	}
}

bool UBaseVacancyCaseObjective::DebugObjectiveState()
{
	return CVarEnableObjectiveLogging.GetValueOnGameThread();
}