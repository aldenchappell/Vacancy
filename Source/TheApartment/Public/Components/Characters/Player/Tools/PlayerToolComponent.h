// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	
	static void Clear()
	{
		FPlayerToolAttachmentStateInfo EmptyState;
		EmptyState.ProgressionComponentClass = nullptr;
		EmptyState.ToolClass = nullptr;
		EmptyState.AttachedTool = nullptr;
		EmptyState.bIsAttached = false;
	}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEAPARTMENT_API UPlayerToolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UPlayerToolComponent();

	UFUNCTION()
	void EquipNewTool(const FPlayerToolAttachmentStateInfo& NewToolState);
	UFUNCTION()
	void UnequipCurrentTool();

protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category="Tools")
	TArray<ABaseTool*> SpawnedTools;
private:
	
	UPROPERTY(VisibleAnywhere, Category="Tool Attachment", meta=(AllowPrivateAccess="true"))
	FPlayerToolAttachmentStateInfo CurrentToolAttachmentState;

	UPROPERTY()
	AVacancyPlayerCharacter* OwningPlayerCharacter = nullptr;

	void UpdateCurrentAttachmentState(const FPlayerToolAttachmentStateInfo& NewAttachmentState);

	UFUNCTION()
	void AddSpawnedTool(ABaseTool* NewSpawnedTool);

	UFUNCTION()
	void RemoveSpawnedTool(ABaseTool* NewSpawnedTool);

	bool AttachToolToSocket(const FPlayerToolAttachmentStateInfo& NewToolState);
public:
	
	bool IsToolEquipped() const;
	ABaseTool* GetEquippedTool() const;
};
