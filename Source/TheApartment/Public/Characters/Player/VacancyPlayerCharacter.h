// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Characters/VacancyCharacter.h"
#include "UI/VacancyHUDData.h"
#include "VacancyPlayerCharacter.generated.h"

class ABaseTool;
class AVacancyHUD;

class UCameraComponent;
class USpringArmComponent;

class UPlayerInteractionComponent;
class UPlayerObjectiveComponent;
class UVacancyAudioListenerComponent;
class UEvidenceInventoryComponent;
class USuspicionReceiverComponent;
class UPlayerToolComponent;
class UPlayerFlashlightComponent;
class UPlayerCameraComponent;
class UPlayerPhoneComponent;
class UPlayerRecorderComponent;

UCLASS()
class THEAPARTMENT_API AVacancyPlayerCharacter : public AVacancyCharacter
{
	GENERATED_BODY()

public:

	AVacancyPlayerCharacter();

	virtual void Tick(float DeltaTime) override;

	// -------------------------------------------------------------------------
	// Animation
	// -------------------------------------------------------------------------

	/**
	 * Updates the player's animation instance based on the currently equipped tool.
	 */
	void UpdateAnimPropsForEquippedTool(const ABaseTool* EquippedTool) const;

	// -------------------------------------------------------------------------
	// HUD Helpers
	// -------------------------------------------------------------------------

	/**
	 * Backwards-compatible wrapper.
	 *
	 * Prefer RefreshHUDElement() in new code.
	 */
	void UpdateHUDByType(const EVacancyHUDElementType& HUDType) const;

	/**
	 * Refreshes a single in-game HUD element.
	 *
	 * Example:
	 * RefreshHUDElement(EVacancyHUDElementType::ToolHUD);
	 */
	UFUNCTION(BlueprintCallable, Category = "Vacancy|HUD")
	void RefreshHUDElement(EVacancyHUDElementType HUDElementType) const;

	/**
	 * Shows or hides a single in-game HUD element.
	 *
	 * Example:
	 * SetHUDElementVisible(EVacancyHUDElementType::CaseInventory, true);
	 */
	UFUNCTION(BlueprintCallable, Category = "Vacancy|HUD")
	void SetHUDElementVisible(EVacancyHUDElementType HUDElementType, bool bVisible) const;

	/**
	 * Refreshes a full HUD screen.
	 *
	 * Example:
	 * RefreshHUDScreen(EVacancyHUDType::InGame);
	 */
	UFUNCTION(BlueprintCallable, Category = "Vacancy|HUD")
	void RefreshHUDScreen(EVacancyHUDType HUDType) const;

	/**
	 * Shows or hides a full HUD screen.
	 *
	 * Example:
	 * SetHUDScreenVisible(EVacancyHUDType::Paused, true);
	 */
	UFUNCTION(BlueprintCallable, Category = "Vacancy|HUD")
	void SetHUDScreenVisible(EVacancyHUDType HUDType, bool bVisible) const;

	/**
	 * Shows one HUD screen and hides the rest.
	 *
	 * Example:
	 * ShowOnlyHUDScreen(EVacancyHUDType::Paused);
	 */
	UFUNCTION(BlueprintCallable, Category = "Vacancy|HUD")
	void ShowOnlyHUDScreen(EVacancyHUDType HUDType) const;

protected:

	virtual void BeginPlay() override;

	// -------------------------------------------------------------------------
	// Camera
	// -------------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> PlayerCamera;

	// -------------------------------------------------------------------------
	// Player State
	// -------------------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	FGameplayTagContainer ActivePlayerTags;

#pragma region Actor Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPlayerInteractionComponent> PlayerInteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPlayerObjectiveComponent> PlayerObjectiveComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UVacancyAudioListenerComponent> AudioListenerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEvidenceInventoryComponent> EvidenceInventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USuspicionReceiverComponent> SuspicionReceiverComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPlayerToolComponent> PlayerToolComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPlayerFlashlightComponent> PlayerFlashlightComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPlayerCameraComponent> PlayerCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPlayerPhoneComponent> PlayerPhoneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPlayerRecorderComponent> PlayerRecorderComponent;

#pragma endregion

private:

	/**
	 * Safely gets the current Vacancy HUD.
	 *
	 * All player HUD helper functions go through this instead of manually finding
	 * and casting widgets.
	 */
	AVacancyHUD* GetVacancyHUD() const;

public:

	UFUNCTION(BlueprintPure, Category = "Player")
	FGameplayTagContainer GetActivePlayerTags() const { return ActivePlayerTags; }

	UFUNCTION(BlueprintPure, Category = "Components")
	FORCEINLINE UPlayerToolComponent* GetPlayerToolComponent() const { return PlayerToolComponent; }
};