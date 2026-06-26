// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Characters/Player/Tools/PlayerToolComponent.h"
#include "GameFramework/Actor.h"
#include "BaseTool.generated.h"

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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tool")
	USkeletalMeshComponent* ToolMesh;

	void SetToolAttachmentStateInfo(const FPlayerToolAttachmentStateInfo& NewToolAttachmentStateInfo)
	{
		ToolAttachmentStateInfo = NewToolAttachmentStateInfo;
	}
protected:
	
	virtual void BeginPlay() override;

private:

	FPlayerToolAttachmentStateInfo ToolAttachmentStateInfo;
};
