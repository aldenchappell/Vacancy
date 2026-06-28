// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Player/VacancyPlayerController.h"

#include "Components/ActorComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Components/Characters/Player/Evidence/EvidenceInventoryComponent.h"
#include "Components/Characters/Player/Interaction/PlayerInteractionComponent.h"
#include "Components/Characters/Player/ProgressionComponents/Camera/PlayerCameraComponent.h"
#include "Components/Characters/Player/ProgressionComponents/Flashlight/PlayerFlashlightComponent.h"
#include "Components/Characters/Player/ProgressionComponents/Phone/PlayerPhoneComponent.h"
#include "Components/Characters/Player/ProgressionComponents/Recorder/PlayerRecorderComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/HUD.h"
#include "UI/VacancyHUD.h"
#include "Utilities/Gameplay/VacancyPlayerUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogVacancyPlayerController, Log, All);

AVacancyPlayerController::AVacancyPlayerController()
{
	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;
}

void AVacancyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!DefaultInputModeName.IsNone())
	{
		SwitchInputMode(DefaultInputModeName);
	}
}

void AVacancyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	/*
	 * This may run before the default input mode is applied.
	 * That is okay. Once SwitchInputMode() runs, bindings are rebuilt.
	 */
	RebuildInputBindings();
}

AVacancyHUD* AVacancyPlayerController::GetVacancyHUD() const
{
	if (AHUD* CurrentHUD = GetHUD())
	{
		return Cast<AVacancyHUD>(CurrentHUD);
	}

	return nullptr;
}

bool AVacancyPlayerController::SwitchInputMode(FName NewInputModeName)
{
	if (NewInputModeName.IsNone())
	{
		UE_LOG(LogVacancyPlayerController, Warning, TEXT("SwitchInputMode failed: input mode name is None."));
		return false;
	}

	if (!InputModeTable)
	{
		UE_LOG(LogVacancyPlayerController, Error, TEXT("SwitchInputMode failed: InputModeTable is not assigned."));
		return false;
	}

	const FString ContextString = FString::Printf(
		TEXT("%s switching to input mode %s"),
		*GetName(),
		*NewInputModeName.ToString()
	);

	const FVacancyInputModeRow* FoundRow = InputModeTable->FindRow<FVacancyInputModeRow>(
		NewInputModeName,
		ContextString,
		true
	);

	if (!FoundRow)
	{
		UE_LOG(
			LogVacancyPlayerController,
			Error,
			TEXT("SwitchInputMode failed: row '%s' was not found in InputModeTable."),
			*NewInputModeName.ToString()
		);

		return false;
	}

	UInputMappingContext* PreviousMappingContext = CurrentMappingContext.Get();

	CurrentInputModeName = NewInputModeName;
	CurrentInputMode = *FoundRow;
	CurrentMappingContext = CurrentInputMode.MappingContext;

	/*
	 * Sprint should never remain stuck while switching modes.
	 */
	SetSprinting(false);

	ApplyCurrentInputMode(PreviousMappingContext);
	RebuildInputBindings();
	UpdateMovementSpeed();

	UE_LOG(
		LogVacancyPlayerController,
		Log,
		TEXT("Switched input mode to '%s'."),
		*CurrentInputModeName.ToString()
	);

	return true;
}

FName AVacancyPlayerController::GetCurrentInputModeName() const
{
	return CurrentInputModeName;
}

bool AVacancyPlayerController::IsSprinting() const
{
	return bIsSprinting;
}

bool AVacancyPlayerController::IsCameraActive() const
{
	return bCameraActive;
}

bool AVacancyPlayerController::IsHiding() const
{
	return bHiding;
}

void AVacancyPlayerController::SetHidingState(bool bNewHiding)
{
	if (bHiding == bNewHiding)
	{
		return;
	}

	bHiding = bNewHiding;
	SetSprinting(false);

	if (bHiding)
	{
		SwitchInputModeIfAvailable(HiddenInputModeName);
	}
	else
	{
		SwitchInputModeIfAvailable(DefaultInputModeName);
	}
}

bool AVacancyPlayerController::HasInputMode(FName InputModeName) const
{
	if (InputModeName.IsNone() || !InputModeTable)
	{
		return false;
	}

	const FString ContextString = FString::Printf(
		TEXT("%s checking input mode %s"),
		*GetName(),
		*InputModeName.ToString()
	);

	return InputModeTable->FindRow<FVacancyInputModeRow>(
		InputModeName,
		ContextString,
		false
	) != nullptr;
}

bool AVacancyPlayerController::SwitchInputModeIfAvailable(FName InputModeName)
{
	if (!HasInputMode(InputModeName))
	{
		return false;
	}

	return SwitchInputMode(InputModeName);
}

void AVacancyPlayerController::ApplyCurrentInputMode(UInputMappingContext* PreviousMappingContext)
{
	if (!IsLocalController())
	{
		return;
	}

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer)
	{
		UE_LOG(LogVacancyPlayerController, Warning, TEXT("ApplyCurrentInputMode failed: LocalPlayer is null."));
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!InputSubsystem)
	{
		UE_LOG(LogVacancyPlayerController, Error, TEXT("ApplyCurrentInputMode failed: Enhanced Input subsystem is null."));
		return;
	}

	if (CurrentInputMode.bClearExistingMappings)
	{
		InputSubsystem->ClearAllMappings();
	}
	else if (PreviousMappingContext && PreviousMappingContext != CurrentMappingContext)
	{
		InputSubsystem->RemoveMappingContext(PreviousMappingContext);
	}

	if (CurrentMappingContext)
	{
		InputSubsystem->AddMappingContext(
			CurrentMappingContext,
			CurrentInputMode.MappingPriority
		);
	}
	else
	{
		UE_LOG(
			LogVacancyPlayerController,
			Warning,
			TEXT("Input mode '%s' has no MappingContext assigned."),
			*CurrentInputModeName.ToString()
		);
	}

	/*
	 * Prevent held keys from leaking between modes.
	 *
	 * Example:
	 * Hold Shift to sprint, open menu, release Shift, close menu.
	 * Without this, sprint can sometimes stay active.
	 */
	FlushPressedKeys();

	ApplyMouseSettings();
}

void AVacancyPlayerController::ApplyMouseSettings()
{
	if (!IsLocalController())
	{
		return;
	}

	bShowMouseCursor = CurrentInputMode.bShowCursor;
	bEnableClickEvents = CurrentInputMode.bEnableClickEvents;
	bEnableMouseOverEvents = CurrentInputMode.bEnableMouseOverEvents;

	switch (CurrentInputMode.MouseInputMode)
	{
	case EVacancyMouseInputMode::GameOnly:
		{
			FInputModeGameOnly InputMode;
			SetInputMode(InputMode);
			break;
		}

	case EVacancyMouseInputMode::GameAndUI:
		{
			FInputModeGameAndUI InputMode;
			InputMode.SetHideCursorDuringCapture(false);
			SetInputMode(InputMode);
			break;
		}

	case EVacancyMouseInputMode::UIOnly:
		{
			FInputModeUIOnly InputMode;
			SetInputMode(InputMode);
			break;
		}

	default:
		{
			FInputModeGameOnly InputMode;
			SetInputMode(InputMode);
			break;
		}
	}
}

void AVacancyPlayerController::RebuildInputBindings()
{
	if (!InputComponent)
	{
		return;
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent)
	{
		UE_LOG(
			LogVacancyPlayerController,
			Error,
			TEXT("InputComponent is not an EnhancedInputComponent. Check Project Settings > Input > Default Input Component Class.")
		);

		return;
	}

	EnhancedInputComponent->ClearActionEventBindings();

	if (CurrentInputMode.MoveAction)
	{
		EnhancedInputComponent->BindAction(
			CurrentInputMode.MoveAction,
			ETriggerEvent::Triggered,
			this,
			&AVacancyPlayerController::HandleMove
		);
	}

	if (CurrentInputMode.LookAction)
	{
		EnhancedInputComponent->BindAction(
			CurrentInputMode.LookAction,
			ETriggerEvent::Triggered,
			this,
			&AVacancyPlayerController::HandleLook
		);
	}

	if (CurrentInputMode.SprintAction)
	{
		EnhancedInputComponent->BindAction(
			CurrentInputMode.SprintAction,
			ETriggerEvent::Started,
			this,
			&AVacancyPlayerController::HandleSprintStarted
		);

		EnhancedInputComponent->BindAction(
			CurrentInputMode.SprintAction,
			ETriggerEvent::Completed,
			this,
			&AVacancyPlayerController::HandleSprintCompleted
		);

		EnhancedInputComponent->BindAction(
			CurrentInputMode.SprintAction,
			ETriggerEvent::Canceled,
			this,
			&AVacancyPlayerController::HandleSprintCompleted
		);
	}

	if (CurrentInputMode.InteractAction)
	{
		EnhancedInputComponent->BindAction(
			CurrentInputMode.InteractAction,
			ETriggerEvent::Started,
			this,
			&AVacancyPlayerController::HandleInteractStarted
		);
	}

	if (CurrentInputMode.CrouchAction)
	{
		EnhancedInputComponent->BindAction(
			CurrentInputMode.CrouchAction,
			ETriggerEvent::Started,
			this,
			&AVacancyPlayerController::HandleCrouchStarted
		);
	}

	if (CurrentInputMode.FlashlightAction)
	{
		EnhancedInputComponent->BindAction(
			CurrentInputMode.FlashlightAction,
			ETriggerEvent::Started,
			this,
			&AVacancyPlayerController::HandleFlashlightStarted
		);
	}

	if (CurrentInputMode.CameraAction)
	{
		EnhancedInputComponent->BindAction(
			CurrentInputMode.CameraAction,
			ETriggerEvent::Started,
			this,
			&AVacancyPlayerController::HandleCameraStarted
		);
	}

	if (CurrentInputMode.TakePhotoAction)
	{
		EnhancedInputComponent->BindAction(
			CurrentInputMode.TakePhotoAction,
			ETriggerEvent::Started,
			this,
			&AVacancyPlayerController::HandleTakePhotoStarted
		);
	}

	if (CurrentInputMode.RecorderAction)
	{
		EnhancedInputComponent->BindAction(
			CurrentInputMode.RecorderAction,
			ETriggerEvent::Started,
			this,
			&AVacancyPlayerController::HandleRecorderStarted
		);
	}

	if (CurrentInputMode.PhoneAction)
	{
		EnhancedInputComponent->BindAction(
			CurrentInputMode.PhoneAction,
			ETriggerEvent::Started,
			this,
			&AVacancyPlayerController::HandlePhoneStarted
		);
	}

	if (CurrentInputMode.CaseFileAction)
	{
		EnhancedInputComponent->BindAction(
			CurrentInputMode.CaseFileAction,
			ETriggerEvent::Started,
			this,
			&AVacancyPlayerController::HandleCaseFileStarted
		);
	}

	if (CurrentInputMode.HideExitAction)
	{
		EnhancedInputComponent->BindAction(
			CurrentInputMode.HideExitAction,
			ETriggerEvent::Started,
			this,
			&AVacancyPlayerController::HandleHideExitStarted
		);
	}

	if (CurrentInputMode.PauseAction)
	{
		EnhancedInputComponent->BindAction(
			CurrentInputMode.PauseAction,
			ETriggerEvent::Started,
			this,
			&AVacancyPlayerController::HandlePauseStarted
		);
	}
}

void AVacancyPlayerController::HandleMove(const FInputActionValue& Value)
{
	if (!CurrentInputMode.bAllowMovement)
	{
		return;
	}

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

	const FVector2D MoveInput = Value.Get<FVector2D>();
	if (MoveInput.IsNearlyZero())
	{
		return;
	}

	/*
	 * First-person movement should only use yaw.
	 *
	 * Looking up or down should not push the player upward/downward.
	 */
	const FRotator ControlRot = GetControlRotation();
	const FRotator YawRot(0.f, ControlRot.Yaw, 0.f);

	const FVector ForwardDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

	ControlledPawn->AddMovementInput(ForwardDir, MoveInput.Y);
	ControlledPawn->AddMovementInput(RightDir, MoveInput.X);
}

void AVacancyPlayerController::HandleLook(const FInputActionValue& Value)
{
	if (!CurrentInputMode.bAllowLook)
	{
		return;
	}

	const FVector2D LookInput = Value.Get<FVector2D>();
	if (LookInput.IsNearlyZero())
	{
		return;
	}

	/*
	 * Mouse X controls yaw.
	 * Mouse Y controls pitch.
	 *
	 * For normal first-person controls:
	 * - Moving mouse right should look right.
	 * - Moving mouse up should look up.
	 *
	 * Unreal's mouse Y direction often feels inverted when passed directly into
	 * AddPitchInput(), so we flip it by default.
	 *
	 * bInvertLookY then intentionally reverses that behavior.
	 */
	const float PitchInput = CurrentInputMode.bInvertLookY
		? LookInput.Y
		: -LookInput.Y;

	AddYawInput(LookInput.X * CurrentInputMode.LookSensitivity);
	AddPitchInput(PitchInput * CurrentInputMode.LookSensitivity);
}

void AVacancyPlayerController::HandleSprintStarted(const FInputActionValue& Value)
{
	SetSprinting(true);
}

void AVacancyPlayerController::HandleSprintCompleted(const FInputActionValue& Value)
{
	SetSprinting(false);
}

void AVacancyPlayerController::HandleInteractStarted(const FInputActionValue& Value)
{
	/*
	 * If the player is currently hiding, E should exit hide instead of doing
	 * normal interaction behavior.
	 */
	if (bHiding)
	{
		OnHideExitPressed();
		return;
	}

	OnInteractPressed();
}

void AVacancyPlayerController::HandleCrouchStarted(const FInputActionValue& Value)
{
	OnCrouchPressed();
}

void AVacancyPlayerController::HandleFlashlightStarted(const FInputActionValue& Value)
{
	OnFlashlightPressed();
}

void AVacancyPlayerController::HandleCameraStarted(const FInputActionValue& Value)
{
	OnCameraPressed();
}

void AVacancyPlayerController::HandleTakePhotoStarted(const FInputActionValue& Value)
{
	OnTakePhotoPressed();
}

void AVacancyPlayerController::HandleRecorderStarted(const FInputActionValue& Value)
{
	OnRecorderPressed();
}

void AVacancyPlayerController::HandlePhoneStarted(const FInputActionValue& Value)
{
	OnPhonePressed();
}

void AVacancyPlayerController::HandleCaseFileStarted(const FInputActionValue& Value)
{
	OnCaseFilePressed();
}

void AVacancyPlayerController::HandleHideExitStarted(const FInputActionValue& Value)
{
	OnHideExitPressed();
}

void AVacancyPlayerController::HandlePauseStarted(const FInputActionValue& Value)
{
	OnPausePressed();
}

void AVacancyPlayerController::SetSprinting(bool bNewSprinting)
{
	AVacancyPlayerCharacter* ControlledCharacter = GetControlledCharacter();

	const bool bCanSprint =
		bNewSprinting &&
		CurrentInputMode.bAllowMovement &&
		CurrentInputMode.bAllowSprint &&
		ControlledCharacter &&
		!ControlledCharacter->bIsCrouched &&
		!bHiding &&
		!bPhoneOpen &&
		!bCaseFileOpen &&
		!bPauseOpen;

	bIsSprinting = bCanSprint;

	UpdateMovementSpeed();
}

void AVacancyPlayerController::UpdateMovementSpeed()
{
	UCharacterMovementComponent* MovementComponent = GetControlledMovementComponent();
	if (!MovementComponent)
	{
		return;
	}

	MovementComponent->MaxWalkSpeed = bIsSprinting
		? CurrentInputMode.SprintSpeed
		: CurrentInputMode.WalkSpeed;

	MovementComponent->MaxWalkSpeedCrouched = CurrentInputMode.CrouchSpeed;
}

AVacancyPlayerCharacter* AVacancyPlayerController::GetControlledCharacter() const
{
	return Cast<AVacancyPlayerCharacter>(GetPawn());
}

UCharacterMovementComponent* AVacancyPlayerController::GetControlledMovementComponent() const
{
	const ACharacter* ControlledCharacter = GetControlledCharacter();
	return ControlledCharacter ? ControlledCharacter->GetCharacterMovement() : nullptr;
}

void AVacancyPlayerController::OnInteractPressed_Implementation()
{
	const UPlayerInteractionComponent* InteractionComponent = UVacancyPlayerUtils::GetPlayerComponent<UPlayerInteractionComponent>(GetControlledCharacter());
	
	if (!InteractionComponent)
	{
		UE_LOG(
			LogVacancyPlayerController,
			Warning,
			TEXT("OnInteractPressed failed: UPlayerInteractionComponent not found on pawn.")
		);
		return;
	}

	const bool bInteractionSuccessful = InteractionComponent->TryInteractWithActiveInteractable();
	if (!bInteractionSuccessful)
	{
		UE_LOG(
			LogVacancyPlayerController,
			Warning,
			TEXT("OnInteractPressed failed: no active interactable or interaction failed.")
		);
	}
	else
	{
		UE_LOG(
			LogVacancyPlayerController,
			Log,
			TEXT("OnInteractPressed: interaction successful.")
		);
	}
}

void AVacancyPlayerController::OnCrouchPressed_Implementation()
{
	if (!CurrentInputMode.bAllowCrouch)
	{
		return;
	}

	ACharacter* ControlledCharacter = GetControlledCharacter();
	if (!ControlledCharacter)
	{
		return;
	}

	SetSprinting(false);

	if (ControlledCharacter->bIsCrouched)
	{
		ControlledCharacter->UnCrouch();
	}
	else
	{
		ControlledCharacter->Crouch();
	}

	UpdateMovementSpeed();
}

void AVacancyPlayerController::OnFlashlightPressed_Implementation()
{
	UPlayerFlashlightComponent* FlashlightComp = UVacancyPlayerUtils::GetPlayerComponent<UPlayerFlashlightComponent>(GetControlledCharacter());
	
	if (!FlashlightComp)
	{
		UE_LOG(
			LogVacancyPlayerController,
			Warning,
			TEXT("OnFlashlightPressed failed: UPlayerFlashlightComponent not found on pawn.")
		);
		return;
	}
	
	FlashlightComp->ToggleComponentToolState(!bFlashlightOn);
	bFlashlightOn = !bFlashlightOn;
}

void AVacancyPlayerController::OnCameraPressed_Implementation()
{
	SetSprinting(false);

	UPlayerCameraComponent* CameraComp = UVacancyPlayerUtils::GetPlayerComponent<UPlayerCameraComponent>(GetControlledCharacter());
	
	if (!CameraComp)
	{
		UE_LOG(
			LogVacancyPlayerController,
			Warning,
			TEXT("OnCameraPressed failed: UPlayerCameraComponent not found on pawn.")
		);
		return;
	}
	
	CameraComp->ToggleComponentToolState(!bCameraActive);
	bCameraActive = !bCameraActive;
	
	if (bCameraActive)
	{
		SwitchInputModeIfAvailable(CameraInputModeName);
	}
	else
	{
		SwitchInputModeIfAvailable(DefaultInputModeName);
	}
}

void AVacancyPlayerController::OnTakePhotoPressed_Implementation()
{
	if (!bCameraActive)
	{
		return;
	}

	const UPlayerCameraComponent* CameraComp = UVacancyPlayerUtils::GetPlayerComponent<UPlayerCameraComponent>(GetControlledCharacter());
	if (!CameraComp)
	{
		UE_LOG(
			LogVacancyPlayerController,
			Warning,
			TEXT("OnTakePhotoPressed failed: UPlayerCameraComponent not found on pawn.")
		);
		return;
	}
	
	const bool bPhotoTaken = CameraComp->TryTakePhoto();
	if (!bPhotoTaken)
	{
		UE_LOG(
			LogVacancyPlayerController,
			Warning,
			TEXT("OnTakePhotoPressed failed: photo could not be taken.")
		);
	}
	else
	{
		UE_LOG(
			LogVacancyPlayerController,
			Log,
			TEXT("OnTakePhotoPressed: photo taken successfully.")
		);
	}
}

void AVacancyPlayerController::OnRecorderPressed_Implementation()
{


	UPlayerRecorderComponent* RecorderComp = UVacancyPlayerUtils::GetPlayerComponent<UPlayerRecorderComponent>(GetControlledCharacter());
	
	if (!RecorderComp)
	{
		UE_LOG(
			LogVacancyPlayerController,
			Warning,
			TEXT("OnRecorderPressed failed: UPlayerRecorderComponent not found on pawn.")
		);
		return;
	}
	
	const bool bRecorderStarted = RecorderComp->TryToggleRecorder(!bRecorderActive);
	if (!bRecorderStarted)
	{
		UE_LOG(
			LogVacancyPlayerController,
			Warning,
			TEXT("OnRecorderPressed failed: recorder could not be toggled.")
		);
	}
	else
	{
		UE_LOG(
			LogVacancyPlayerController,
			Log,
			TEXT("OnRecorderPressed: recorder toggled successfully.")
		);
		bRecorderActive = !bRecorderActive;
	}
}

void AVacancyPlayerController::OnPhonePressed_Implementation()
{
	SetSprinting(false);

	UPlayerPhoneComponent* PhoneComp = UVacancyPlayerUtils::GetPlayerComponent<UPlayerPhoneComponent>(GetControlledCharacter());
	if (!PhoneComp)
	{
		UE_LOG(
			LogVacancyPlayerController,
			Warning,
			TEXT("OnPhonePressed failed: UPlayerPhoneComponent not found on pawn.")
		);
		return;
	}
	
	const bool bToggledPhone = PhoneComp->ToggleComponentToolState(!bPhoneOpen);
	if (!bToggledPhone)
	{
		UE_LOG(
			LogVacancyPlayerController,
			Warning,
			TEXT("OnPhonePressed failed: phone could not be toggled.")
		);
		return;
	}
	else
	{
		UE_LOG(
			LogVacancyPlayerController,
			Log,
			TEXT("OnPhonePressed: phone toggled successfully.")
		);
		bPhoneOpen = !bPhoneOpen;
	}

	if (bPhoneOpen)
	{
		SwitchInputModeIfAvailable(PhoneInputModeName);
	}
	else
	{
		SwitchInputModeIfAvailable(DefaultInputModeName);
	}
}

void AVacancyPlayerController::OnCaseFilePressed_Implementation()
{
	SetSprinting(false);

	APawn* ControlledPawn = GetPawn();
	if (!IsValid(ControlledPawn))
	{
		UE_LOG(LogVacancyPlayerController, Warning, TEXT("OnCaseFilePressed failed: pawn is invalid."));
		return;
	}

	UEvidenceInventoryComponent* EvidenceInventoryComp = UVacancyPlayerUtils::GetPlayerComponent<UEvidenceInventoryComponent>(GetControlledCharacter());

	if (!IsValid(EvidenceInventoryComp))
	{
		UE_LOG(
			LogVacancyPlayerController,
			Warning,
			TEXT("OnCaseFilePressed failed: UEvidenceInventoryComponent not found on pawn.")
		);
		return;
	}

	const bool bDesiredOpenState = !bCaseFileOpen;

	const bool bCanToggleEvidenceInventory = EvidenceInventoryComp->TryToggleInventory(bDesiredOpenState);
	if (!bCanToggleEvidenceInventory)
	{
		UE_LOG(
			LogVacancyPlayerController,
			Warning,
			TEXT("OnCaseFilePressed failed: evidence inventory could not be toggled.")
		);
		return;
	}

	bCaseFileOpen = bDesiredOpenState;

	if (bCaseFileOpen)
	{
		SwitchInputModeIfAvailable(CaseFileInputModeName);
	}
	else
	{
		SwitchInputModeIfAvailable(DefaultInputModeName);
	}
}

void AVacancyPlayerController::OnHideExitPressed_Implementation()
{
	if (!bHiding)
	{
		OnInteractPressed();
		return;
	}

	const UPlayerInteractionComponent* InteractionComponent = UVacancyPlayerUtils::GetPlayerComponent<UPlayerInteractionComponent>(GetControlledCharacter());
	
	if (!InteractionComponent)
	{
		UE_LOG(
			LogVacancyPlayerController,
			Warning,
			TEXT("OnHideExitPressed failed: UPlayerInteractionComponent not found on pawn.")
		);
		return;
	}

	InteractionComponent->ExitHide();

	SetHidingState(false);
}

void AVacancyPlayerController::OnPausePressed_Implementation()
{
	bPauseOpen = !bPauseOpen;

	SetSprinting(false);

	if (bPauseOpen)
	{
		InputModeBeforePause = CurrentInputModeName;

		if (bUseEnginePause)
		{
			SetPause(true);
		}

		SwitchInputModeIfAvailable(PauseInputModeName);
	}
	else
	{
		if (bUseEnginePause)
		{
			SetPause(false);
		}

		const FName ReturnMode = InputModeBeforePause.IsNone()
			? DefaultInputModeName
			: InputModeBeforePause;

		SwitchInputModeIfAvailable(ReturnMode);

		InputModeBeforePause = NAME_None;
	}
}