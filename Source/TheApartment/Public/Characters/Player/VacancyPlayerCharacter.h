// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Characters/VacancyCharacter.h"
#include "UI/VacancyHUDData.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "VacancyPlayerCharacter.generated.h"

class UVacancyAbilitySystemComponent;
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

class UInputAction;
class UVacancyGameplayAbility;
class UAbilitySystemComponent;
struct FInputActionValue;

/**
 * Connects one Enhanced Input action to one GAS input tag.
 */
USTRUCT(BlueprintType)
struct FVacancyAbilityInputBinding
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Input")
	TObjectPtr<UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Input")
	FGameplayTag InputTag;

	bool IsValid() const
	{
		return InputAction != nullptr && InputTag.IsValid();
	}
};

UCLASS()
class THEAPARTMENT_API AVacancyPlayerCharacter : public AVacancyCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AVacancyPlayerCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent) override;

	/**
	 * Required by IAbilitySystemInterface.
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure, Category = "Vacancy|Abilities")
	UVacancyAbilitySystemComponent* GetVacancyAbilitySystemComponent() const
	{
		return AbilitySystemComponent;
	}

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

	/**
	 * Abilities automatically granted when the player begins play.
	 *
	 * Tool abilities that are unlocked later can be granted by their progression
	 * or acquisition systems instead.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vacancy|Abilities")
	TArray<TSubclassOf<UVacancyGameplayAbility>> StartupAbilities;

	/**
	 * Enhanced Input actions and the GAS input tags they represent.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vacancy|Abilities|Input")
	TArray<FVacancyAbilityInputBinding> AbilityInputBindings;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UVacancyAbilitySystemComponent> AbilitySystemComponent;

#pragma endregion

private:

	/**
	 * Safely gets the current Vacancy HUD.
	 *
	 * All player HUD helper functions go through this instead of manually finding
	 * and casting widgets.
	 */
	AVacancyHUD* GetVacancyHUD() const;

	void InitializeAbilitySystem();
	void GrantStartupAbilities();

	void HandleAbilityInputPressed(
		const FInputActionValue& InputValue,
		FGameplayTag InputTag);

	void HandleAbilityInputReleased(
		const FInputActionValue& InputValue,
		FGameplayTag InputTag);
	
public:

	UFUNCTION(BlueprintPure, Category = "Player")
	FGameplayTagContainer GetActivePlayerTags() const;

	UFUNCTION(BlueprintPure, Category = "Components")
	FORCEINLINE UPlayerToolComponent* GetPlayerToolComponent() const { return PlayerToolComponent; }

	UFUNCTION(BlueprintPure, Category = "Components")
	UPlayerFlashlightComponent* GetPlayerFlashlightComponent() const
	{
		return PlayerFlashlightComponent;
	}

	UFUNCTION(BlueprintPure, Category = "Components")
	UPlayerCameraComponent* GetPlayerCameraComponent() const
	{
		return PlayerCameraComponent;
	}

	UFUNCTION(BlueprintPure, Category = "Components")
	UPlayerRecorderComponent* GetPlayerRecorderComponent() const
	{
		return PlayerRecorderComponent;
	}

	UFUNCTION(BlueprintPure, Category = "Components")
	UPlayerPhoneComponent* GetPlayerPhoneComponent() const
	{
		return PlayerPhoneComponent;
	}
};