#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Data/VacancyAbilityData.h"
#include "VacancyGameplayAbility.generated.h"

class AVacancyPlayerCharacter;
class UVacancyAbilityDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnVacancyAbilityActivated,
	AVacancyPlayerCharacter*,
	ActivatingPlayer);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnVacancyAbilityEnded,
	AVacancyPlayerCharacter*,
	EndingPlayer,
	bool,
	bWasCancelled);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnVacancyAbilityCancelled,
	AVacancyPlayerCharacter*,
	CancellingPlayer);

/**
 * Base class for all gameplay abilities used by Vacancy.
 *
 * Responsibilities:
 * - Resolve the owning player.
 * - Validate the configured data asset.
 * - Process required and blocking tags.
 * - Apply and remove granted runtime tags.
 * - Commit the ability.
 * - Forward activation, input, cancellation, and ending events to Blueprint.
 * - Implement Instant, Hold, Toggle, and Manual input behavior.
 *
 * This class does not tick.
 */
UCLASS(Abstract, Blueprintable)
class THEAPARTMENT_API UVacancyGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UVacancyGameplayAbility();

	/**
	 * Returns a Blueprint-friendly copy of the configured ability data.
	 */
	UFUNCTION(BlueprintPure, Category = "Vacancy|Abilities")
	FVacancyAbilityData GetVacancyAbilityData() const;

	/**
	 * Returns the current player avatar associated with this ability.
	 */
	UFUNCTION(BlueprintPure, Category = "Vacancy|Abilities")
	AVacancyPlayerCharacter* GetVacancyPlayer() const;

	/**
	 * C++ accessor that avoids copying ability data.
	 *
	 * Returns nullptr when no valid data asset is assigned.
	 */
	const FVacancyAbilityData* GetVacancyAbilityDataPtr() const;

	UPROPERTY(BlueprintAssignable, Category = "Vacancy|Abilities")
	FOnVacancyAbilityActivated OnVacancyAbilityActivated;

	UPROPERTY(BlueprintAssignable, Category = "Vacancy|Abilities")
	FOnVacancyAbilityEnded OnVacancyAbilityEnded;

	UPROPERTY(BlueprintAssignable, Category = "Vacancy|Abilities")
	FOnVacancyAbilityCancelled OnVacancyAbilityCancelled;

protected:

	// -------------------------------------------------------------------------
	// GAS lifecycle
	// -------------------------------------------------------------------------

	virtual void OnAvatarSet(
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilitySpec& Spec) override;

	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;

	virtual void CancelAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateCancelAbility) override;

	virtual void InputPressed(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual void InputReleased(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;

	// -------------------------------------------------------------------------
	// Vacancy activation validation
	// -------------------------------------------------------------------------

	/**
	 * Additional game-specific activation check.
	 *
	 * Override this in a C++ or Blueprint child when the ability requires
	 * conditions not represented by gameplay tags.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Vacancy|Abilities")
	bool CanActivateVacancyAbility(
		AVacancyPlayerCharacter* ActivatingPlayer) const;

	virtual bool CanActivateVacancyAbility_Implementation(
		AVacancyPlayerCharacter* ActivatingPlayer) const;

	// -------------------------------------------------------------------------
	// Blueprint lifecycle events
	// -------------------------------------------------------------------------

	/**
	 * Called when this ability receives its player avatar.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Vacancy|Abilities")
	void OnAbilityGranted(AVacancyPlayerCharacter* GrantedPlayer);

	virtual void OnAbilityGranted_Implementation(
		AVacancyPlayerCharacter* GrantedPlayer);

	/**
	 * Called after the ability has successfully committed.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Vacancy|Abilities")
	void OnAbilityActivated(AVacancyPlayerCharacter* ActivatingPlayer);

	virtual void OnAbilityActivated_Implementation(
		AVacancyPlayerCharacter* ActivatingPlayer);

	/**
	 * Called whenever the ability ends.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Vacancy|Abilities")
	void OnAbilityEnded(
		AVacancyPlayerCharacter* EndingPlayer,
		bool bWasCancelled);

	virtual void OnAbilityEnded_Implementation(
		AVacancyPlayerCharacter* EndingPlayer,
		bool bWasCancelled);

	/**
	 * Called immediately before GAS processes cancellation.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Vacancy|Abilities")
	void OnAbilityCancelled(AVacancyPlayerCharacter* CancellingPlayer);

	virtual void OnAbilityCancelled_Implementation(
		AVacancyPlayerCharacter* CancellingPlayer);

	UFUNCTION(BlueprintNativeEvent, Category = "Vacancy|Abilities")
	void OnAbilityInputPressed(AVacancyPlayerCharacter* PressingPlayer);

	virtual void OnAbilityInputPressed_Implementation(
		AVacancyPlayerCharacter* PressingPlayer);

	UFUNCTION(BlueprintNativeEvent, Category = "Vacancy|Abilities")
	void OnAbilityInputReleased(AVacancyPlayerCharacter* ReleasingPlayer);

	virtual void OnAbilityInputReleased_Implementation(
		AVacancyPlayerCharacter* ReleasingPlayer);

	// -------------------------------------------------------------------------
	// Configuration
	// -------------------------------------------------------------------------

	/**
	 * Static data describing this ability.
	 *
	 * This is intentionally a hard reference. Ability data is small and should
	 * already be available when the ability is granted or activated.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vacancy|Abilities")
	TObjectPtr<UVacancyAbilityDataAsset> VacancyAbilityDataAsset = nullptr;

	/**
	 * Controls whether the ability is instant, held, toggled, or manually ended.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Vacancy|Abilities")
	EVacancyAbilityInputBehavior InputBehavior =
		EVacancyAbilityInputBehavior::Instant;

#pragma region Debug
	
	static bool DebugAbility();

#pragma endregion

private:

	AVacancyPlayerCharacter* ResolvePlayer(
		const FGameplayAbilityActorInfo* ActorInfo) const;

	void ApplyGrantedTags(const FGameplayAbilityActorInfo* ActorInfo);
	void RemoveGrantedTags(const FGameplayAbilityActorInfo* ActorInfo);

	

	UPROPERTY(Transient)
	TObjectPtr<AVacancyPlayerCharacter> OwnerPlayer = nullptr;

	bool bGrantedTagsApplied = false;
};