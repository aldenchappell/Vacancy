// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerToolComponent.generated.h"

USTRUCT(BlueprintType)
struct FPlayerToolAttachmentStateInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tool Attachment")
	AActor* AttachedTool = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tool Attachment")
	bool bIsAttached = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToolEquipped, const FPlayerToolAttachmentStateInfo&, ToolAttachmentStateInfo);
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEAPARTMENT_API UPlayerToolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UPlayerToolComponent();

	UFUNCTION()
	void EquipTool(const FPlayerToolAttachmentStateInfo& ToolAttachmentStateInfo);
	UFUNCTION()
	void UnequipCurrentTool();
	
	UPROPERTY(BlueprintAssignable, Category="Tool Attachment")
	FOnToolEquipped OnToolEquipped;
	
protected:
	
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(VisibleAnywhere, Category="Tool Attachment")
	FPlayerToolAttachmentStateInfo CurrentToolAttachmentState;

public:
	
	bool IsToolEquipped() const;
	AActor* GetEquippedTool() const;
};
