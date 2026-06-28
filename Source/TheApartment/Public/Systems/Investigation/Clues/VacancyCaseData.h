#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "VacancyCaseData.generated.h"

class UBaseVacancyClue;
class UVacancyCaseDataAsset;

USTRUCT(BlueprintType)
struct FVacancyCaseDisplayData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Case Data")
	FText CaseDisplayName = FText::FromString("Case Name");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Case Data")
	FText CaseDisplayDescription = FText::FromString("Case Description");
};

UENUM(BlueprintType)
enum class EVacancyClueTier : uint8
{
	Low UMETA(DisplayName = "Low"),
	Medium UMETA(DisplayName = "Medium"),
	High UMETA(DisplayName = "High")
};

USTRUCT(BlueprintType)
struct FVacancyEvidenceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Evidence Data")
	UTexture2D* EvidenceDisplayIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Evidence Data")
	FText EvidenceDisplayTitle = FText::FromString("Evidence Name");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Evidence Data")
	FText EvidenceDisplayName = FText::FromString("Evidence Name");
};

USTRUCT(BlueprintType)
struct FVacancyClueSummaryData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Clue Data")
	FName ClueID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Clue Data")
	FText ClueDisplaySummaryText = FText::FromString("Clue Summary");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Clue Data")
	EVacancyClueTier ClueImportance = EVacancyClueTier::Low;
};

UENUM(BlueprintType)
enum class EVacancyCaseFileStatus : uint8
{
	Open,
	Closed,
	MAX
};

USTRUCT(BlueprintType)
struct FVacancyCaseFileSuspectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Case File Data")
	FText SuspectDisplayName = FText::FromString("Suspect Name");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Case File Data")
	int32 SuspectDisplayAge = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Case File Data")
	FText SuspectDisplayOccupation = FText::FromString("Suspect Occupation");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Case File Data")
	FDateTime SuspectDisplayLastSeen = FDateTime::Now();
};

USTRUCT(BlueprintType)
struct FVacancyCaseFileData
{
	GENERATED_BODY()

	/*
	 * Display location for the case file, used to identify the case in the in-game case file UI 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Case File Data")
	FText CaseFileDisplayLocation = FText::FromString("Case File Location");

	/*
	 * Display ID for the case file, used to identify the case in the in-game case file UI 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Case File Data")
	FText CaseDisplayID = FText::FromString("Case ID");

	/*
	 * Status of the case file, used to determine if the case is open or closed
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Case File Data")
	EVacancyCaseFileStatus CaseFileStatus = EVacancyCaseFileStatus::Open;

	/*
	 * Suspect data for the case, used to populate the in-game case file UI 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Case File Data")
	FVacancyCaseFileSuspectData CaseFileSuspectData;

	/*
	 * Display data for the case, used to populate the in-game case file UI 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Case Data")
	FVacancyCaseDisplayData CaseDisplayData;

	/*
	 * Clue data classes for cases
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Case Data")
	TArray<TSubclassOf<UBaseVacancyClue>> ClueDataClasses;
};

UENUM(BlueprintType)
enum class EVacancyCaseObjectiveStatus : uint8
{
	InProgress,
	Completed,
	Failed,
	MAX
};

USTRUCT(BlueprintType)
struct FVacancyCaseObjectiveStateData
{
	GENERATED_BODY()

	/*
	 * Unique identifier for the objective, used to track progress and completion status
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Objective Data")
	FName ObjectiveID = NAME_None;

	/*
	 * Display text for the objective, used to show the player what they need to do
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Objective Data")
	FText ObjectiveDisplayText = FText::FromString("Objective Text");

	/*
	 * Whether the objective is optional or required for case completion
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Objective Data")
	bool bIsOptional = false;

	/*
	 * Whether the objective has been completed by the player
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Objective Data")
	bool bIsObjectiveCompleted = false;

	/*
	 * Status of the objective, used to determine if the objective is in progress, completed, or failed
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Objective Data")
	EVacancyCaseObjectiveStatus ObjectiveStatus = EVacancyCaseObjectiveStatus::MAX;
};

USTRUCT(BlueprintType)
struct FVacancyCaseObjectiveData
{
	GENERATED_BODY()

	/*
	 * Gameplay tag for the objective, used to categorize and filter objectives in the game
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Objective Data")
	FGameplayTag ObjectiveTag;
	
	/*
	 * Data for the objective, including its ID, display text, and completion status
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Objective Data")
	TArray<FVacancyCaseObjectiveStateData> Objectives;
};

USTRUCT(BlueprintType)
struct FVacancyClueDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Clue Data")
	TSoftObjectPtr<UVacancyCaseDataAsset> ClueCaseDataAsset = nullptr;
};