// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "VacancyPlayerController.generated.h"

class AVacancyHUD;
class UInputAction;
class UInputMappingContext;
class UCharacterMovementComponent;

/**
 * Basic mouse/input behavior for each input mode.
 *
 * For this first-person horror controller, the expected modes are:
 * - Gameplay: locked mouse, no cursor
 * - Camera: locked mouse, no cursor, photo input enabled
 * - Menu / Phone / Case File: cursor visible
 * - Hidden: limited input, usually only Exit Hide
 */
UENUM(BlueprintType)
enum class EVacancyMouseInputMode : uint8
{
	GameOnly UMETA(DisplayName = "Game Only"),
	GameAndUI UMETA(DisplayName = "Game and UI"),
	UIOnly UMETA(DisplayName = "UI Only")
};

/**
 * DataTable row defining one complete input mode.
 *
 * Each row represents a mode such as:
 * - Gameplay
 * - Camera
 * - Phone
 * - CaseFile
 * - Hidden
 * - Menu
 *
 * The row name is what you pass into SwitchInputMode().
 */
USTRUCT(BlueprintType)
struct THEAPARTMENT_API FVacancyInputModeRow : public FTableRowBase
{
	GENERATED_BODY()

	// -------------------------------------------------------------------------
	// Enhanced Input Mapping
	// -------------------------------------------------------------------------

	/**
	 * Enhanced Input Mapping Context used by this mode.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Mapping")
	TObjectPtr<UInputMappingContext> MappingContext = nullptr;

	/**
	 * Priority for this mapping context.
	 *
	 * Higher priority mappings override lower priority mappings.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Mapping")
	int32 MappingPriority = 0;

	/**
	 * If true, all previous mapping contexts are removed before this one is added.
	 *
	 * For a simple horror game, this should usually be true.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Mapping")
	bool bClearExistingMappings = true;

	// -------------------------------------------------------------------------
	// Core First-Person Actions
	// -------------------------------------------------------------------------

	/**
	 * WASD movement.
	 *
	 * Expected value type: Axis2D
	 * X = right/left
	 * Y = forward/backward
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Actions")
	TObjectPtr<UInputAction> MoveAction = nullptr;

	/**
	 * Mouse look.
	 *
	 * Expected value type: Axis2D
	 * X = yaw
	 * Y = pitch
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Actions")
	TObjectPtr<UInputAction> LookAction = nullptr;

	/**
	 * E: interact with doors, notes, usable objects, hiding spots, etc.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Actions")
	TObjectPtr<UInputAction> InteractAction = nullptr;

	/**
	 * Ctrl: toggle crouch.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Actions")
	TObjectPtr<UInputAction> CrouchAction = nullptr;

	/**
	 * Shift: hold sprint.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Actions")
	TObjectPtr<UInputAction> SprintAction = nullptr;

	// -------------------------------------------------------------------------
	// Horror Utility Actions
	// -------------------------------------------------------------------------

	/**
	 * F: toggle flashlight.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Actions")
	TObjectPtr<UInputAction> FlashlightAction = nullptr;

	/**
	 * Right Mouse / C: toggle camera.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Actions")
	TObjectPtr<UInputAction> CameraAction = nullptr;

	/**
	 * Left Mouse: take photo.
	 *
	 * This is ignored unless the controller considers the camera active.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Actions")
	TObjectPtr<UInputAction> TakePhotoAction = nullptr;

	/**
	 * R: toggle recorder.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Actions")
	TObjectPtr<UInputAction> RecorderAction = nullptr;

	/**
	 * Tab: toggle phone.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Actions")
	TObjectPtr<UInputAction> PhoneAction = nullptr;

	/**
	 * J: toggle case file/evidence inventory.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Actions")
	TObjectPtr<UInputAction> CaseFileAction = nullptr;

	/**
	 * E: exit hiding.
	 *
	 * This can use the same Input Action asset as InteractAction, but do not assign
	 * both InteractAction and HideExitAction in the same DataTable row.
	 *
	 * Example:
	 * - Gameplay row uses InteractAction.
	 * - Hidden row uses HideExitAction.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Actions")
	TObjectPtr<UInputAction> HideExitAction = nullptr;

	/**
	 * Escape: pause/menu.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Actions")
	TObjectPtr<UInputAction> PauseAction = nullptr;

	// -------------------------------------------------------------------------
	// Mode Rules
	// -------------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Rules")
	bool bAllowMovement = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Rules")
	bool bAllowLook = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Rules")
	bool bAllowSprint = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Rules")
	bool bAllowCrouch = true;

	// -------------------------------------------------------------------------
	// Movement Values
	// -------------------------------------------------------------------------

	/**
	 * Slower default speed for first-person horror pacing.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Movement", meta = (ClampMin = "0.0"))
	float WalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Movement", meta = (ClampMin = "0.0"))
	float SprintSpeed = 425.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Movement", meta = (ClampMin = "0.0"))
	float CrouchSpeed = 125.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Movement", meta = (ClampMin = "0.0"))
	float LookSensitivity = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Movement")
	bool bInvertLookY = false;

	// -------------------------------------------------------------------------
	// Mouse / UI
	// -------------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Mouse")
	EVacancyMouseInputMode MouseInputMode = EVacancyMouseInputMode::GameOnly;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Mouse")
	bool bShowCursor = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Mouse")
	bool bEnableClickEvents = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mode|Mouse")
	bool bEnableMouseOverEvents = false;
};

/**
 * First-person horror PlayerController.
 *
 * Responsibilities:
 * - DataTable-driven input mode switching
 * - Enhanced Input context management
 * - Basic movement/look/sprint/crouch
 * - Input routing for flashlight, camera, photo, recorder, phone, case file, hiding, and pause
 *
 * This controller intentionally does not implement replication.
 */
UCLASS()
class THEAPARTMENT_API AVacancyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AVacancyPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// -------------------------------------------------------------------------
	// HUD
	// -------------------------------------------------------------------------

	UFUNCTION(BlueprintPure, Category = "HUD")
	AVacancyHUD* GetVacancyHUD() const;

	// -------------------------------------------------------------------------
	// Input Modes
	// -------------------------------------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Input Mode")
	bool SwitchInputMode(FName NewInputModeName);

	UFUNCTION(BlueprintPure, Category = "Input Mode")
	FName GetCurrentInputModeName() const;

	// -------------------------------------------------------------------------
	// State
	// -------------------------------------------------------------------------

	UFUNCTION(BlueprintPure, Category = "Player State")
	bool IsSprinting() const;

	UFUNCTION(BlueprintPure, Category = "Player State")
	bool IsCameraActive() const;

	UFUNCTION(BlueprintPure, Category = "Player State")
	bool IsHiding() const;

	/**
	 * Call this from your hiding system when the player enters/exits a hiding spot.
	 *
	 * Example:
	 * - Player interacts with closet
	 * - Interaction component moves player into hiding
	 * - Interaction component calls SetHidingState(true)
	 */
	UFUNCTION(BlueprintCallable, Category = "Player State")
	void SetHidingState(bool bNewHiding);

	// -------------------------------------------------------------------------
	// Blueprint Hooks
	// -------------------------------------------------------------------------

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void OnInteractPressed();
	virtual void OnInteractPressed_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void OnCrouchPressed();
	virtual void OnCrouchPressed_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void OnFlashlightPressed();
	virtual void OnFlashlightPressed_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void OnCameraPressed();
	virtual void OnCameraPressed_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void OnTakePhotoPressed();
	virtual void OnTakePhotoPressed_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void OnRecorderPressed();
	virtual void OnRecorderPressed_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void OnPhonePressed();
	virtual void OnPhonePressed_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void OnCaseFilePressed();
	virtual void OnCaseFilePressed_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void OnHideExitPressed();
	virtual void OnHideExitPressed_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void OnPausePressed();
	virtual void OnPausePressed_Implementation();

protected:

	// -------------------------------------------------------------------------
	// Input Mode Data
	// -------------------------------------------------------------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Mode")
	TObjectPtr<UDataTable> InputModeTable = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Mode")
	FName DefaultInputModeName = TEXT("Gameplay");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Mode")
	FName CameraInputModeName = TEXT("Camera");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Mode")
	FName PhoneInputModeName = TEXT("Phone");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Mode")
	FName CaseFileInputModeName = TEXT("CaseFile");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Mode")
	FName HiddenInputModeName = TEXT("Hidden");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Mode")
	FName PauseInputModeName = TEXT("Menu");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pause")
	bool bUseEnginePause = true;

private:

	// -------------------------------------------------------------------------
	// Input Mode Internals
	// -------------------------------------------------------------------------

	bool HasInputMode(FName InputModeName) const;
	bool SwitchInputModeIfAvailable(FName InputModeName);

	void ApplyCurrentInputMode(UInputMappingContext* PreviousMappingContext);
	void ApplyMouseSettings();
	void RebuildInputBindings();

	// -------------------------------------------------------------------------
	// Input Handlers
	// -------------------------------------------------------------------------

	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);

	void HandleSprintStarted(const FInputActionValue& Value);
	void HandleSprintCompleted(const FInputActionValue& Value);

	void HandleInteractStarted(const FInputActionValue& Value);
	void HandleCrouchStarted(const FInputActionValue& Value);
	void HandleFlashlightStarted(const FInputActionValue& Value);
	void HandleCameraStarted(const FInputActionValue& Value);
	void HandleTakePhotoStarted(const FInputActionValue& Value);
	void HandleRecorderStarted(const FInputActionValue& Value);
	void HandlePhoneStarted(const FInputActionValue& Value);
	void HandleCaseFileStarted(const FInputActionValue& Value);
	void HandleHideExitStarted(const FInputActionValue& Value);
	void HandlePauseStarted(const FInputActionValue& Value);

	// -------------------------------------------------------------------------
	// Movement Helpers
	// -------------------------------------------------------------------------

	void SetSprinting(bool bNewSprinting);
	void UpdateMovementSpeed();

	ACharacter* GetControlledCharacter() const;
	UCharacterMovementComponent* GetControlledMovementComponent() const;

	
	UPROPERTY(Transient)
	FVacancyInputModeRow CurrentInputMode;

	UPROPERTY(Transient)
	FName CurrentInputModeName = NAME_None;

	UPROPERTY(Transient)
	TObjectPtr<UInputMappingContext> CurrentMappingContext = nullptr;

	FName InputModeBeforePause = NAME_None;

	bool bIsSprinting = false;
	bool bCameraActive = false;
	bool bFlashlightOn = false;
	bool bRecorderActive = false;
	bool bPhoneOpen = false;
	bool bCaseFileOpen = false;
	bool bPauseOpen = false;
	bool bHiding = false;

	template <typename T>
	T* GetPawnComponent() const
	{
		APawn* ControlledPawn = GetPawn();
		if (!IsValid(ControlledPawn))
		{
			return nullptr;
		}

		return Cast<T>(ControlledPawn->GetComponentByClass(T::StaticClass()));
	}
};