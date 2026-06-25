// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTool.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tool")
	USkeletalMeshComponent* ToolMesh;

	
protected:
	
	virtual void BeginPlay() override;


};
