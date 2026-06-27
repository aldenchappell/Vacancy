#pragma once

#include "CoreMinimal.h"
#include "Systems/Items/Tools/ToolData.h"
#include "VacancyPlayerAnimData.generated.h"

USTRUCT(BlueprintType)
struct FVacancyPlayerAnimData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	EToolType EquippedToolType = EToolType::MAX;
	
};