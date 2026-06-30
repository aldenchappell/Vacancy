// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Characters/VacancyCharacter.h"
#include "UI/VacancyHUDData.h"
#include "VacancyPlayerCharacter.generated.h"


class UVacancyUserWidgetBase;
class ABaseTool;

UCLASS()
class THEAPARTMENT_API AVacancyPlayerCharacter : public AVacancyCharacter
{
	GENERATED_BODY()

public:
	AVacancyPlayerCharacter();
	virtual void Tick(float DeltaTime) override;

	void UpdateAnimPropsForEquippedTool(const ABaseTool* EquippedTool) const;

	void UpdateHUDByType(const EVacancyHUDType& HUDType) const;
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
	class UPlayerObjectiveComponent* PlayerObjectiveComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UVacancyAudioListenerComponent* AudioListenerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UEvidenceInventoryComponent* EvidenceInventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USuspicionReceiverComponent* SuspicionReceiverComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPlayerToolComponent* PlayerToolComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPlayerFlashlightComponent* PlayerFlashlightComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPlayerCameraComponent* PlayerCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPlayerPhoneComponent* PlayerPhoneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UPlayerRecorderComponent* PlayerRecorderComponent;
	
#pragma endregion

private:
	static bool ValidateHUDWidget(const UVacancyUserWidgetBase* HUDWidget);
	
public:
	
	UFUNCTION(BlueprintPure)
	FGameplayTagContainer GetActivePlayerTags() const { return ActivePlayerTags; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UPlayerToolComponent* GetPlayerToolComponent() const { return PlayerToolComponent; }
};
