// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/Objectives/VacancyObjectiveUtils.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Components/Characters/Player/PlayerObjectiveComponent/PlayerObjectiveComponent.h"
#include "Systems/Investigation/Objectives/BaseVacancyCaseObjective.h"
#include "Engine/Blueprint.h"
#include "UObject/SoftObjectPath.h"
#include "Misc/PackageName.h"
#include "Utilities/Gameplay/VacancyPlayerUtils.h"

UBaseVacancyCaseObjective* UVacancyObjectiveUtils::SpawnObjective(
	const UObject* WorldContextObject,
	const TSubclassOf<UBaseVacancyCaseObjective> ObjectiveClass)
{
	if (!IsValid(WorldContextObject))
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnObjective called with an invalid WorldContextObject."));
		return nullptr;
	}

	if (!IsValid(ObjectiveClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnObjective called with an invalid ObjectiveClass."));
		return nullptr;
	}

	UObject* ObjectiveOuter = const_cast<UObject*>(WorldContextObject);

	UBaseVacancyCaseObjective* NewObjective =
		NewObject<UBaseVacancyCaseObjective>(ObjectiveOuter, ObjectiveClass);

	if (!IsValid(NewObjective))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("SpawnObjective failed to create objective from class %s."),
			*GetNameSafe(ObjectiveClass)
		);

		return nullptr;
	}

	NewObjective->InitializeObjective();
	return NewObjective;
}

UBaseVacancyCaseObjective* UVacancyObjectiveUtils::GetObjectiveByID(
	const TArray<TObjectPtr<UBaseVacancyCaseObjective>>& Objectives,
	const FName& ObjectiveID)
{
	if (ObjectiveID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetObjectiveByID failed: ObjectiveID is None."));
		return nullptr;
	}

	for (UBaseVacancyCaseObjective* Objective : Objectives)
	{
		if (!IsValid(Objective))
		{
			continue;
		}

		if (Objective->GetObjectiveID() == ObjectiveID)
		{
			return Objective;
		}
	}

	return nullptr;
}

FName UVacancyObjectiveUtils::GetObjectiveID(const UBaseVacancyCaseObjective* Objective, const int32 ObjectiveIndex)
{
	if (!IsValid(Objective))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetObjectiveID called with an invalid Objective."));
		return NAME_None;
	}

	if (Objective->GetObjectivesStateData().Num() <= ObjectiveIndex)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("GetObjectiveID: ObjectiveIndex %d is out of bounds for Objective with ID %s."),
			ObjectiveIndex, *Objective->GetObjectiveID().ToString());
		return NAME_None;
	}

	return Objective->GetObjectiveID();
}

bool UVacancyObjectiveUtils::TryActivateObjectiveByID(UBaseVacancyCaseObjective* Objective, const int32 ObjectiveIndex,
	const FName& ObjectiveID, const AVacancyPlayerCharacter* PlayerCharacter)
{
	if (!IsValid(Objective))
	{
		UE_LOG(LogTemp, Warning, TEXT("TryActivateObjectiveByID called with an invalid Objective."));
		return false;
	}

	if (ObjectiveID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("TryActivateObjectiveByID called with an invalid ObjectiveID."));
		return false;
	}

	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("TryActivateObjectiveByID called with an invalid PlayerCharacter."));
		return false;
	}

	if (const FName CurrentObjectiveID = GetObjectiveID(Objective, ObjectiveIndex);
		CurrentObjectiveID != ObjectiveID)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("TryActivateObjectiveByID: ObjectiveID %s does not match the current objective's ID %s."),
			*ObjectiveID.ToString(), *CurrentObjectiveID.ToString());
		return false;
	}

	if (IsObjectiveActive(Objective, ObjectiveIndex, ObjectiveID))
	{
		return true; // Already active
	}

	if (const EVacancyCaseObjectiveStatus CurrentStatus = GetObjectiveState(Objective, ObjectiveIndex, ObjectiveID);
		CurrentStatus == EVacancyCaseObjectiveStatus::Completed)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("TryActivateObjectiveByID: Objective %s is already completed and cannot be activated."),
			*Objective->GetName());
		return false;
	}

	UPlayerObjectiveComponent* PlayerObjectiveComponent =
		UVacancyPlayerUtils::GetPlayerComponent<UPlayerObjectiveComponent>(PlayerCharacter);
	
	if (!IsValid(PlayerObjectiveComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("TryActivateObjectiveByID: PlayerCharacter %s does not have a valid PlayerObjectiveComponent."), *PlayerCharacter->GetName());
		return false;
	}

	PlayerObjectiveComponent->SetActiveObjective(Objective);

	Objective->MarkObjectiveAsActive(PlayerCharacter);
	return true;
}

bool UVacancyObjectiveUtils::IsObjectiveActive(const UBaseVacancyCaseObjective* Objective, const int32 ObjectiveIndex, const FName& ObjectiveID)
{
	if (!IsValid(Objective))
	{
		UE_LOG(LogTemp, Warning, TEXT("IsObjectiveActive called with an invalid Objective."));
		return false;
	}

	if (ObjectiveID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("IsObjectiveActive called with an invalid ObjectiveID."));
		return false;
	}

	const EVacancyCaseObjectiveStatus ObjectiveStatus =
		GetObjectiveState(Objective, ObjectiveIndex, ObjectiveID);

	return ObjectiveStatus == EVacancyCaseObjectiveStatus::Active;
}

bool UVacancyObjectiveUtils::IsObjectiveComplete(const UBaseVacancyCaseObjective* Objective, const int32 ObjectiveIndex, const FName& ObjectiveID)
{
	if (!IsValid(Objective))
	{
		UE_LOG(LogTemp, Warning, TEXT("IsObjectiveComplete called with an invalid Objective."));
		return false;
	}

	if (ObjectiveID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("IsObjectiveComplete called with an invalid ObjectiveID."));
		return false;
	}

	const EVacancyCaseObjectiveStatus ObjectiveStatus =
		GetObjectiveState(Objective, ObjectiveIndex, ObjectiveID);

	return ObjectiveStatus == EVacancyCaseObjectiveStatus::Completed;
}

EVacancyCaseObjectiveStatus UVacancyObjectiveUtils::GetObjectiveState(const UBaseVacancyCaseObjective* Objective, const int32 ObjectiveIndex,
	const FName& ObjectiveID)
{
	if (!IsValid(Objective))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetObjectiveState called with an invalid Objective."));
		return EVacancyCaseObjectiveStatus::MAX;
	}

	if (ObjectiveID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetObjectiveState called with an invalid ObjectiveID."));
		return EVacancyCaseObjectiveStatus::MAX;
	}

	return Objective->GetObjectivesStateData().Num() > 0 ?
		Objective->GetObjectiveStatus() :
		EVacancyCaseObjectiveStatus::MAX;
}

TArray<TSoftClassPtr<UBaseVacancyCaseObjective>> UVacancyObjectiveUtils::FindObjectiveClassesInFolder(
	const FString& FolderPath)
{
	TArray<TSoftClassPtr<UBaseVacancyCaseObjective>> ObjectiveClasses;

	if (FolderPath.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("FindObjectiveClassesInFolder failed: FolderPath is empty."));
		return ObjectiveClasses;
	}

	if (!FolderPath.StartsWith(TEXT("/Game/")))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("FindObjectiveClassesInFolder failed: invalid folder path '%s'. Use a path like /Game/Vacancy/Systems/Objectives/Objectives/Level_01."),
			*FolderPath
		);

		return ObjectiveClasses;
	}

	FAssetRegistryModule& AssetRegistryModule =
		FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	/*
	 * Scan this folder now.
	 *
	 * This is important when searching by folder at runtime, especially in PIE
	 * or when assets have not already been discovered by the registry.
	 */
	TArray<FString> PathsToScan;
	PathsToScan.Add(FolderPath);

	AssetRegistry.ScanPathsSynchronous(PathsToScan, true);

	FARFilter Filter;
	Filter.PackagePaths.Add(FName(*FolderPath));
	Filter.bRecursivePaths = true;

	/*
	 * Objective Blueprints are UBlueprint assets in the content browser.
	 * The generated class inside the Blueprint is what actually derives from
	 * UBaseVacancyCaseObjective.
	 */
	Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());

	TArray<FAssetData> FoundBlueprintAssets;
	AssetRegistry.GetAssets(Filter, FoundBlueprintAssets);

	for (const FAssetData& AssetData : FoundBlueprintAssets)
	{
		FString GeneratedClassPathString;

		if (!AssetData.GetTagValue(TEXT("GeneratedClass"), GeneratedClassPathString))
		{
			continue;
		}

		/*
		 * GeneratedClass tag usually comes back as an export text path.
		 * Convert it into a normal object path.
		 *
		 * Example:
		 * BlueprintGeneratedClass'/Game/.../BP_Objective.BP_Objective_C'
		 *
		 * becomes:
		 * /Game/.../BP_Objective.BP_Objective_C
		 */
		const FString GeneratedClassObjectPath =
			FPackageName::ExportTextPathToObjectPath(GeneratedClassPathString);

		if (GeneratedClassObjectPath.IsEmpty())
		{
			continue;
		}

		const FSoftClassPath SoftClassPath(GeneratedClassObjectPath);
		TSoftClassPtr<UBaseVacancyCaseObjective> ObjectiveClass(SoftClassPath);

		ObjectiveClasses.Add(ObjectiveClass);
	}

	UE_LOG(
		LogTemp,
		Log,
		TEXT("FindObjectiveClassesInFolder: Found %d possible objective Blueprint class(es) in folder: %s"),
		ObjectiveClasses.Num(),
		*FolderPath
	);

	return ObjectiveClasses;
}
