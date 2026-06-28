// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Characters/Player/PlayerObjectiveComponent/PlayerObjectiveComponent.h"

#include "Systems/Investigation/Objectives/BaseVacancyCaseObjective.h"
#include "Utilities/Objectives/VacancyObjectiveUtils.h"

UPlayerObjectiveComponent::UPlayerObjectiveComponent()
{

}

void UPlayerObjectiveComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializePlayerObjectives();
}

bool UPlayerObjectiveComponent::AddActiveObjective(UBaseVacancyCaseObjective* NewObjective)
{
	if (!IsValid(NewObjective))
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to add an invalid objective to PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
		return false;
	}

	if (ActiveObjectives.Contains(NewObjective))
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to add an already active objective to PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
		return false;
	}

	ActiveObjectives.Add(NewObjective);
	return true;
}

bool UPlayerObjectiveComponent::RemoveActiveObjective(UBaseVacancyCaseObjective* ObjectiveToRemove)
{
	if (!IsValid(ObjectiveToRemove))
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to remove an invalid objective from PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
		return false;
	}

	if (!ActiveObjectives.Contains(ObjectiveToRemove))
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to remove a non-active objective from PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
		return false;
	}

	return ActiveObjectives.Remove(ObjectiveToRemove) > 0;
}

bool UPlayerObjectiveComponent::TryBeginObjectiveByID(const FName& ObjectiveID) const
{
	if (ObjectiveID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("TryBeginObjectiveByID called with an invalid ObjectiveID on PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
		return false;
	}

	if (UBaseVacancyCaseObjective* ObjectiveToStart = UVacancyObjectiveUtils::GetObjectiveByID(ActiveObjectives, ObjectiveID))
	{
		if (ObjectiveToStart->SetObjectiveState(ObjectiveID, EVacancyCaseObjectiveStatus::InProgress))
		{
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to set objective state to Active for ObjectiveID: %s on PlayerObjectiveComponent on %s"), *ObjectiveID.ToString(), *GetOwner()->GetName());
			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No active objective found with ObjectiveID: %s on PlayerObjectiveComponent on %s"), *ObjectiveID.ToString(), *GetOwner()->GetName());
		return false;
	}
}

bool UPlayerObjectiveComponent::TryCompleteObjectiveByID(const FName& ObjectiveID) const
{
	if (ObjectiveID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("TryCompleteObjectiveByID called with an invalid ObjectiveID on PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
		return false;
	}

	if (UBaseVacancyCaseObjective* ObjectiveToComplete = UVacancyObjectiveUtils::GetObjectiveByID(ActiveObjectives, ObjectiveID))
	{
		

		
		if (ObjectiveToComplete->SetObjectiveState(ObjectiveID, EVacancyCaseObjectiveStatus::Completed))
		{
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to set objective state to Completed for ObjectiveID: %s on PlayerObjectiveComponent on %s"), *ObjectiveID.ToString(), *GetOwner()->GetName());
			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No active objective found with ObjectiveID: %s on PlayerObjectiveComponent on %s"), *ObjectiveID.ToString(), *GetOwner()->GetName());
		return false;
	}
}

bool UPlayerObjectiveComponent::TryFailObjectiveByID(const FName& ObjectiveID, const FString& FailReason) const
{
	if (ObjectiveID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("TryFailObjectiveByID called with an invalid ObjectiveID on PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
		return false;
	}

	if (UBaseVacancyCaseObjective* ObjectiveToFail = UVacancyObjectiveUtils::GetObjectiveByID(ActiveObjectives, ObjectiveID))
	{
		if (ObjectiveToFail->SetObjectiveState(ObjectiveID, EVacancyCaseObjectiveStatus::Failed))
		{
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to set objective state to Failed for ObjectiveID: %s on PlayerObjectiveComponent on %s"), *ObjectiveID.ToString(), *GetOwner()->GetName());
			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No active objective found with ObjectiveID: %s on PlayerObjectiveComponent on %s"), *ObjectiveID.ToString(), *GetOwner()->GetName());
		return false;
	}
}

TArray<UBaseVacancyCaseObjective*> UPlayerObjectiveComponent::GetActiveObjectives() const
{
	if (ActiveObjectives.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No active objectives found for PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
	}

	for (const UBaseVacancyCaseObjective* Objective : ActiveObjectives)
	{
		if (IsValid(Objective) && UVacancyObjectiveUtils::IsObjectiveActive(Objective, 0, Objective->GetObjectives()[0].ObjectiveID))
		{
			return ActiveObjectives;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("No active objectives found for PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
	return TArray<UBaseVacancyCaseObjective*>();
}

TArray<UBaseVacancyCaseObjective*> UPlayerObjectiveComponent::GetCompletedObjectives() const
{
	if (CompletedObjectives.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No completed objectives found for PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
	}

	for (const UBaseVacancyCaseObjective* Objective : ActiveObjectives)
	{
		if (IsValid(Objective) && UVacancyObjectiveUtils::IsObjectiveComplete(Objective, 0, Objective->GetObjectives()[0].ObjectiveID))
		{
			return CompletedObjectives;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("No completed objectives found for PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
	return TArray<UBaseVacancyCaseObjective*>();
}

void UPlayerObjectiveComponent::InitializePlayerObjectives()
{
	if (DefaultObjectives.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No default objectives set for PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
		return;
	}

	for (const TSubclassOf<UBaseVacancyCaseObjective> ObjectiveClass : DefaultObjectives)
	{
		if (!IsValid(ObjectiveClass))
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid objective class in DefaultObjectives for PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
			continue;
		}

		UVacancyObjectiveUtils::SpawnObjective(this, ObjectiveClass);
	}
}
