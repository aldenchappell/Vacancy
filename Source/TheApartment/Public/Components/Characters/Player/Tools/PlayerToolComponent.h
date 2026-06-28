// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Systems/Items/Tools/ToolData.h"
#include "PlayerToolComponent.generated.h"

class UBasePlayerProgressionComponent;
class AVacancyPlayerCharacter;
class ABaseTool;

USTRUCT(BlueprintType)
struct FPlayerToolAttachmentStateInfo
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tool Attachment")
	TSubclassOf<UBasePlayerProgressionComponent> ProgressionComponentClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tool Attachment")
	TSubclassOf<ABaseTool> ToolClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tool Attachment")
	FName ToolAttachSocket = NAME_None;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tool Attachment")
	ABaseTool* AttachedTool = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tool Attachment")
	bool bIsAttached = false;
	
	static void Clear(FPlayerToolAttachmentStateInfo& ToolStateInfo)
	{
		ToolStateInfo.ProgressionComponentClass = nullptr;
		ToolStateInfo.ToolClass = nullptr;
		ToolStateInfo.AttachedTool = nullptr;
		ToolStateInfo.bIsAttached = false;
		ToolStateInfo.ToolAttachSocket = NAME_None;
	}
};



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEAPARTMENT_API UPlayerToolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UPlayerToolComponent();

	UFUNCTION()
	bool EquipNewTool(const FPlayerToolAttachmentStateInfo& NewToolState);
	UFUNCTION()
	bool UnequipCurrentTool();

protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category="Tools")
	TArray<ABaseTool*> SpawnedTools;
private:
	
	UPROPERTY(VisibleAnywhere, Category="Tool Attachment", meta=(AllowPrivateAccess="true"))
	FPlayerToolAttachmentStateInfo CurrentToolAttachmentState;

	UPROPERTY()
	AVacancyPlayerCharacter* OwningPlayerCharacter = nullptr;

	UPROPERTY()
	EToolType CurrentToolType = EToolType::MAX;

	void UpdateCurrentAttachmentState(const FPlayerToolAttachmentStateInfo& NewAttachmentState);

	UFUNCTION()
	void AddSpawnedTool(ABaseTool* NewSpawnedTool);

	UFUNCTION()
	void RemoveSpawnedTool(ABaseTool* NewSpawnedTool);

	bool AttachToolToSocket(const FPlayerToolAttachmentStateInfo& NewToolState) const;
public:
	
	bool IsToolEquipped() const;
	ABaseTool* GetEquippedTool() const;
};
