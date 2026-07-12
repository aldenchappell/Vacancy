#pragma once

#include "NativeGameplayTags.h"

/**
 * Native gameplay tags used by Vacancy.
 *
 * These tags are registered when the TheApartment module starts.
 * They do not need to be manually added to DefaultGameplayTags.ini.
 */
namespace VacancyGameplayTags
{
	// -------------------------------------------------------------------------
	// Ability identity tags
	// -------------------------------------------------------------------------

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Tool_Flashlight_Toggle);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Tool_Camera_Mode);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Tool_Camera_TakePhoto);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Tool_Recorder_Toggle);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Tool_Phone_Toggle);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Utility_CaseFile_Toggle);

	// -------------------------------------------------------------------------
	// Ability input tags
	// -------------------------------------------------------------------------

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Ability_Flashlight);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Ability_CameraMode);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Ability_CameraPrimary);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Ability_Recorder);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Ability_Phone);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Ability_CaseFile);

	// -------------------------------------------------------------------------
	// Tool state tags
	// -------------------------------------------------------------------------

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Tool_Flashlight_Active);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Tool_Camera_Active);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Tool_Recorder_Active);

	// -------------------------------------------------------------------------
	// UI state tags
	// -------------------------------------------------------------------------

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_UI_Phone_Open);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_UI_CaseFile_Open);

	// -------------------------------------------------------------------------
	// General player state tags
	// -------------------------------------------------------------------------

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Player_Disabled);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Player_Hiding);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Player_InCinematic);
}