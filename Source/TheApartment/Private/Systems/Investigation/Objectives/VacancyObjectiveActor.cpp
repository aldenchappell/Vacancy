// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/Investigation/Objectives/VacancyObjectiveActor.h"

#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Systems/Investigation/Objectives/BaseVacancyCaseObjective.h"

static bool bLogObjectiveActor = false;

static TAutoConsoleVariable<bool> CVarLogObjectiveActor(
	TEXT("Vacancy.LogObjectiveActor"),
	false,
	TEXT("Enable logging for AVacancyObjectiveActor."),
	ECVF_Default);

void AVacancyObjectiveActor::SetOwnerObjective(UBaseVacancyCaseObjective* NewOwnerObjective)
{
	if (!IsValid(NewOwnerObjective))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetOwnerObjective: NewOwnerObjective is not valid for %s."), *GetName());
		return;
	}

	OwnerObjective = NewOwnerObjective;
}

void AVacancyObjectiveActor::OnObjectiveCompleted(const UBaseVacancyCaseObjective* CompletedObjective,
	const AVacancyPlayerCharacter* PlayerCharacter)
{
	if (!IsValid(CompletedObjective))
	{
		UE_LOG(LogTemp, Warning, TEXT("OnObjectiveCompleted: CompletedObjective is not valid for %s."), *GetName());
		return;
	}

	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("OnObjectiveCompleted: PlayerCharacter is not valid for %s."), *GetName());
		return;
	}

	Internal_OnObjectiveCompleted(CompletedObjective, PlayerCharacter);
}

void AVacancyObjectiveActor::Internal_OnObjectiveCompleted(const UBaseVacancyCaseObjective* CompletedObjective,
	const AVacancyPlayerCharacter* PlayerCharacter)
{
	if (DebugObjectiveActors())
	{
		UE_LOG(LogTemp, Log, TEXT("Objective %s completed by player %s."), *CompletedObjective->GetName(), *PlayerCharacter->GetName());
	}

}

TSubclassOf<UBaseVacancyCaseObjective> AVacancyObjectiveActor::GetOwnerObjectiveClass() const
{
	if (!IsValid(OwnerObjective))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetOwnerObjectiveClass: OwnerObjective is not valid for %s."), *GetName());
		return nullptr;
	}

	return OwnerObjective->GetClass();
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

void AVacancyObjectiveActor::OnOwnerObjectiveSpawned(UBaseVacancyCaseObjective* SpawnedObjective)
{
	SpawnedObjective->InitializeObjective();
	SpawnedObjective->SetObjectiveOwnerActor(this);
}

bool AVacancyObjectiveActor::DebugObjectiveActors()
{
	return bLogObjectiveActor;
}
