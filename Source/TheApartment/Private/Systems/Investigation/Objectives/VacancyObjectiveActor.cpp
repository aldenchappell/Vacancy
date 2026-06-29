// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Investigation/Objectives/VacancyObjectiveActor.h"

#include "Systems/Investigation/Objectives/BaseVacancyCaseObjective.h"


void AVacancyObjectiveActor::SetOwnerObjective(UBaseVacancyCaseObjective* NewOwnerObjective)
{
	if (!IsValid(NewOwnerObjective))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetOwnerObjective: NewOwnerObjective is not valid for %s."), *GetName());
		return;
	}

	OwnerObjective = NewOwnerObjective;
}

FVector AVacancyObjectiveActor::GetObjectiveLocation() const
{
	if (!IsValid(OwnerObjective))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetObjectiveLocation: OwnerObjective is not valid for %s."), *GetName());
		return FVector::ZeroVector;
	}

	//TODO: Implement logic to determine the objective location based on the OwnerObjective's data.
	if (!OwnerObjective->ShouldDisplayObjective())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetObjectiveLocation: OwnerObjective should not be displayed for %s."), *GetName());
		return FVector::ZeroVector;
	}

	return GetActorLocation();
}
