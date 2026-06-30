// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Characters/Player/PlayerObjectiveComponent/PlayerObjectiveComponent.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
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

void UPlayerObjectiveComponent::SetCurrentObjective(UBaseVacancyCaseObjective* NewObjective)
{
	if (!IsValid(NewObjective))
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to add an invalid objective to PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
		return;
	}
	
	ActiveObjective = NewObjective;
}

void UPlayerObjectiveComponent::ClearActiveObjective()
{
	if (!IsValid(ActiveObjective))
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to clear an invalid active objective from PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
		return;
	}
	
	ActiveObjective = nullptr;
}

void UPlayerObjectiveComponent::SetActiveObjective(UBaseVacancyCaseObjective* NewObjective)
{
	if (!IsValid(NewObjective))
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to set an invalid active objective on PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
		return;
	}

	ActiveObjective = NewObjective;
}

bool UPlayerObjectiveComponent::TryCompleteActiveObjective()
{
	if (!IsValid(ActiveObjective))
	{
		UE_LOG(LogTemp, Warning, TEXT("TryCompleteActiveObjective called with no active objective on PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
		return false;
	}

	ActiveObjective->MarkObjectiveAsCompleted(Cast<AVacancyPlayerCharacter>(GetOwner()));
	CompletedObjectives.Add(ActiveObjective);
	return true;
}

bool UPlayerObjectiveComponent::TryFailActiveObjective(const FString& FailReason) const
{
	if (!IsValid(ActiveObjective))
	{
		UE_LOG(LogTemp, Warning, TEXT("TryFailActiveObjective called with no active objective on PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
		return false;
	}

	ActiveObjective->MarkObjectiveAsFailed(Cast<AVacancyPlayerCharacter>(GetOwner()), FailReason);
	return true;
}

UBaseVacancyCaseObjective* UPlayerObjectiveComponent::GetActiveObjective() const
{
	if (!IsValid(ActiveObjective))
	{
		UE_LOG(LogTemp, Warning, TEXT("No active objective found for PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
		return nullptr;
	}

	return ActiveObjective;
}

TArray<UBaseVacancyCaseObjective*> UPlayerObjectiveComponent::GetCompletedObjectives() const
{
	if (CompletedObjectives.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No completed objectives found for PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
		return TArray<UBaseVacancyCaseObjective*>();
	}

	for (int i = 0; i < CompletedObjectives.Num(); ++i)
	{
		const int32 ObjectiveIndex = i;
		const UBaseVacancyCaseObjective* CompletedObjective = CompletedObjectives[ObjectiveIndex];
		
		if (!IsValid(CompletedObjective))
		{
			UE_LOG(LogTemp, Warning, TEXT("Encountered an invalid completed objective in PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
			continue;
		}
		
		if (!CompletedObjective->IsObjectiveCompleted())
		{
			UE_LOG(LogTemp, Warning,
				TEXT("Objective %s in CompletedObjectives is not marked as completed on PlayerObjectiveComponent on %s"),
				*CompletedObjective->GetName(), *GetOwner()->GetName());
			return TArray<UBaseVacancyCaseObjective*>();
		}
	}

	return CompletedObjectives;
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

bool UPlayerObjectiveComponent::IsObjectiveComplete(const UBaseVacancyCaseObjective* Objective) const
{
	if (!IsValid(Objective))
	{
		UE_LOG(LogTemp, Warning, TEXT("IsObjectiveComplete called with an invalid Objective on PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
		return false;
	}

	for (int i = 0; i < Objective->GetObjectivesStateData().Num(); ++i)
	{
		const int32 ObjectiveIndex = i;

		if (const FName ObjectiveID =
			UVacancyObjectiveUtils::GetObjectiveID(Objective, ObjectiveIndex);
			!UVacancyObjectiveUtils::IsObjectiveComplete(Objective, ObjectiveIndex, ObjectiveID))
		{
			return false;
		}
	}

	return true;
}
