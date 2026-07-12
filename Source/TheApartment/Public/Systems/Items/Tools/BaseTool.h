// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToolData.h"
#include "Components/Characters/Player/Tools/PlayerToolComponent.h"
#include "GameFramework/Actor.h"
#include "Systems/AbilitySystem/Abilities/Data/VacancyAbilityData.h"
#include "BaseTool.generated.h"

struct FVacancyAbilityData;
struct FPlayerToolAttachmentStateInfo;
class AVacancyPlayerCharacter;

UCLASS()
class THEAPARTMENT_API ABaseTool : public AActor
{
	GENERATED_BODY()

public:
	
	ABaseTool();

	UFUNCTION(BlueprintNativeEvent, Category="Tool")
	void OnToolEquipped(AVacancyPlayerCharacter* UnequippingCharacter);
	
	UFUNCTION(BlueprintNativeEvent, Category="Tool")
	void OnToolUnequipped(AVacancyPlayerCharacter* UnequippingCharacter);

	UFUNCTION()
	FName GetToolAttachSocket() const;

	UFUNCTION(BlueprintCallable, Category="Tool")
	FORCEINLINE FToolDataRow GetToolData() const { return ToolData; }

	UFUNCTION(BlueprintCallable, Category="Tool")
	TArray<FVacancyAbilityData> GetToolAbilityDatas() const;

	/**
	 * Gives this tool's configured abilities to the player.
	 *
	 * Giving an already-granted ability is treated as success by the ASC.
	 */
	void GrantToolAbilities(
		const AVacancyPlayerCharacter* ReceivingCharacter) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tool")
	USkeletalMeshComponent* ToolMesh;

	void SetToolAttachmentStateInfo(const FPlayerToolAttachmentStateInfo& NewToolAttachmentStateInfo);

	
	
protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tool")
	UAnimMontage* EquipToolAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tool")
	UAnimMontage* UnequipToolAnim;

	UPROPERTY(EditDefaultsOnly, Category="Tool")
	FToolDataRow ToolData;
	
private:

	FPlayerToolAttachmentStateInfo ToolAttachmentStateInfo;
	FVacancyAbilityData ToolAbilityData;

	static bool DebugTools();
};
