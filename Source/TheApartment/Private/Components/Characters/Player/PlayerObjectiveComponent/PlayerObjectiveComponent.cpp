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

TArray<UBaseVacancyCaseObjective*> UPlayerObjectiveComponent::GetActiveObjectives() const
{
	if (ActiveObjectives.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No active objectives found for PlayerObjectiveComponent on %s"), *GetOwner()->GetName());
	}

	return ActiveObjectives;
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