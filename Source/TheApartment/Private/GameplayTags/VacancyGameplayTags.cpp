#include "GameplayTags/VacancyGameplayTags.h"

namespace VacancyGameplayTags
{
#pragma region Ability identity tags

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Ability_Tool_Flashlight_Toggle,
		"Ability.Tool.Flashlight.Toggle",
		"Turns the player's flashlight on or off.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Ability_Tool_Camera_Mode,
		"Ability.Tool.Camera.Mode",
		"Enters or exits camera mode.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Ability_Tool_Camera_TakePhoto,
		"Ability.Tool.Camera.TakePhoto",
		"Attempts to capture a photograph while camera mode is active.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Ability_Tool_Recorder_Toggle,
		"Ability.Tool.Recorder.Toggle",
		"Starts or stops the player's audio recorder.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Ability_Tool_Phone_Toggle,
		"Ability.Tool.Phone.Toggle",
		"Opens or closes the player's phone.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Ability_Utility_CaseFile_Toggle,
		"Ability.Utility.CaseFile.Toggle",
		"Opens or closes the player's case file.");

#pragma endregion

#pragma region Ability input tags

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Input_Ability_Flashlight,
		"Input.Ability.Flashlight",
		"Input used to toggle the flashlight.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Input_Ability_CameraMode,
		"Input.Ability.CameraMode",
		"Input used to enter or exit camera mode.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Input_Ability_CameraPrimary,
		"Input.Ability.CameraPrimary",
		"Primary camera input used to take a photograph.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Input_Ability_Recorder,
		"Input.Ability.Recorder",
		"Input used to start or stop the recorder.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Input_Ability_Phone,
		"Input.Ability.Phone",
		"Input used to open or close the phone.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Input_Ability_CaseFile,
		"Input.Ability.CaseFile",
		"Input used to open or close the case file.");

#pragma endregion

#pragma region Tool state tags

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		State_Tool_Flashlight_Active,
		"State.Tool.Flashlight.Active",
		"The player's flashlight is currently active.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		State_Tool_Camera_Active,
		"State.Tool.Camera.Active",
		"The player is currently using camera mode.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		State_Tool_Recorder_Active,
		"State.Tool.Recorder.Active",
		"The player's audio recorder is currently recording.");

#pragma endregion
	
#pragma region UI state tags

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		State_UI_Phone_Open,
		"State.UI.Phone.Open",
		"The player's phone interface is currently open.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		State_UI_CaseFile_Open,
		"State.UI.CaseFile.Open",
		"The player's case file interface is currently open.");

	// -------------------------------------------------------------------------
	// General player state tags
	// -------------------------------------------------------------------------

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		State_Player_Disabled,
		"State.Player.Disabled",
		"The player is unable to perform normal gameplay actions.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		State_Player_Hiding,
		"State.Player.Hiding",
		"The player is currently inside a hiding spot.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		State_Player_InCinematic,
		"State.Player.InCinematic",
		"The player is currently controlled by a cinematic.");

#pragma endregion
	
}