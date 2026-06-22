// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Characters/VacancyCharacter.h"
#include "VacancyPlayerCharacter.generated.h"


UCLASS()
class THEAPARTMENT_API AVacancyPlayerCharacter : public AVacancyCharacter
{
	GENERATED_BODY()

public:
	AVacancyPlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleAnywhere)
	FGameplayTagContainer ActivePlayerTags;

#pragma region Actor Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPlayerInteractionComponent* PlayerInteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UVacancyAudioListenerComponent* AudioListenerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UEvidenceInventoryComponent* EvidenceInventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USuspicionReceiverComponent* SuspicionReceiverComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPlayerToolComponent* PlayerToolComponent;
	
#pragma endregion
public:
	
	UFUNCTION(BlueprintPure)
	FGameplayTagContainer GetActivePlayerTags() const { return ActivePlayerTags; }
};
