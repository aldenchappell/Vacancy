#include "Systems/AbilitySystem/Abilities/VacancyGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Characters/Player/VacancyPlayerCharacter.h"
#include "Systems/AbilitySystem/Abilities/Data/Assets/VacancyAbilityDataAsset.h"

static TAutoConsoleVariable<int32> CVarDebugVacancyGameplayAbility(
	TEXT("Vacancy.DebugGameplayAbility"),
	0,
	TEXT("0: Disable Vacancy gameplay ability logs.\n")
	TEXT("1: Enable Vacancy gameplay ability logs."),
	ECVF_Default);

UVacancyGameplayAbility::UVacancyGameplayAbility()
{
	/*
	 * Every Vacancy ability needs its own runtime state because it may retain
	 * an owner, granted-tag state, tool reference, or ability tasks.
	 *
	 * InstancedPerActor creates one reusable instance for the owning actor
	 * instead of allocating a new ability object every activation.
	 */
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UVacancyGameplayAbility::OnAvatarSet(
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	OwnerPlayer = ResolvePlayer(ActorInfo);

	if (!IsValid(OwnerPlayer))
	{
		if (DebugAbility())
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("%s received an invalid or non-player avatar."),
				*GetNameSafe(this));
		}

		return;
	}

	OnAbilityGranted(OwnerPlayer);
}

bool UVacancyGameplayAbility::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(
		Handle,
		ActorInfo,
		SourceTags,
		TargetTags,
		OptionalRelevantTags))
	{
		return false;
	}

	if (ActorInfo == nullptr || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		return false;
	}

	AVacancyPlayerCharacter* ActivatingPlayer = ResolvePlayer(ActorInfo);
	if (!IsValid(ActivatingPlayer))
	{
		return false;
	}

	const FVacancyAbilityData* AbilityData = GetVacancyAbilityDataPtr();
	if (AbilityData == nullptr || !AbilityData->IsValidAbilityData())
	{
		if (DebugAbility())
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("%s cannot activate because its ability data is invalid."),
				*GetNameSafe(this));
		}

		return false;
	}

	const UAbilitySystemComponent* AbilitySystem =
		ActorInfo->AbilitySystemComponent.Get();

	const FVacancyAbilityTagData& TagData = AbilityData->AbilityTagData;

	if (!TagData.ActivationBlockingTags.IsEmpty()
		&& AbilitySystem->HasAnyMatchingGameplayTags(
			TagData.ActivationBlockingTags))
	{
		return false;
	}

	if (!TagData.RequiredActivationTags.IsEmpty()
		&& !AbilitySystem->HasAllMatchingGameplayTags(
			TagData.RequiredActivationTags))
	{
		return false;
	}

	return CanActivateVacancyAbility(ActivatingPlayer);
}

void UVacancyGameplayAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(
		Handle,
		ActorInfo,
		ActivationInfo,
		TriggerEventData);

	OwnerPlayer = ResolvePlayer(ActorInfo);

	if (!IsValid(OwnerPlayer))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	/*
	 * CommitAbility processes any configured cost and cooldown.
	 * Even abilities without costs or cooldowns can safely use this path.
	 */
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		if (DebugAbility())
		{
			UE_LOG(
				LogTemp,
				Warning,
				TEXT("%s failed to commit."),
				*GetNameSafe(this));
		}

		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	ApplyGrantedTags(ActorInfo);

	if (DebugAbility())
	{
		UE_LOG(
			LogTemp,
			Log,
			TEXT("Activated ability %s for %s."),
			*GetNameSafe(this),
			*GetNameSafe(OwnerPlayer));
	}

	OnAbilityActivated(OwnerPlayer);
	OnVacancyAbilityActivated.Broadcast(OwnerPlayer);

	/*
	 * Instant abilities perform their work inside OnAbilityActivated and then
	 * immediately clean themselves up.
	 */
	if (InputBehavior == EVacancyAbilityInputBehavior::Instant && IsActive())
	{
		EndAbility(
			Handle,
			ActorInfo,
			ActivationInfo,
			true,
			false);
	}
}

void UVacancyGameplayAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const bool bReplicateEndAbility,
	const bool bWasCancelled)
{
	AVacancyPlayerCharacter* EndingPlayer = ResolvePlayer(ActorInfo);

	RemoveGrantedTags(ActorInfo);

	if (IsValid(EndingPlayer))
	{
		OnAbilityEnded(EndingPlayer, bWasCancelled);
		OnVacancyAbilityEnded.Broadcast(EndingPlayer, bWasCancelled);
	}

	if (DebugAbility())
	{
		UE_LOG(
			LogTemp,
			Log,
			TEXT("Ended ability %s. Cancelled: %s"),
			*GetNameSafe(this),
			bWasCancelled ? TEXT("true") : TEXT("false"));
	}

	Super::EndAbility(
		Handle,
		ActorInfo,
		ActivationInfo,
		bReplicateEndAbility,
		bWasCancelled);
}

void UVacancyGameplayAbility::CancelAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const bool bReplicateCancelAbility)
{
	AVacancyPlayerCharacter* CancellingPlayer = ResolvePlayer(ActorInfo);

	if (IsValid(CancellingPlayer))
	{
		OnAbilityCancelled(CancellingPlayer);
		OnVacancyAbilityCancelled.Broadcast(CancellingPlayer);
	}

	Super::CancelAbility(
		Handle,
		ActorInfo,
		ActivationInfo,
		bReplicateCancelAbility);
}

void UVacancyGameplayAbility::InputPressed(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	AVacancyPlayerCharacter* PressingPlayer = ResolvePlayer(ActorInfo);

	if (IsValid(PressingPlayer))
	{
		OnAbilityInputPressed(PressingPlayer);
	}

	/*
	 * The first press activates a Toggle ability.
	 * A later press reaches this function because the ability is already active,
	 * so the second press ends it.
	 */
	if (InputBehavior == EVacancyAbilityInputBehavior::Toggle && IsActive())
	{
		EndAbility(
			Handle,
			ActorInfo,
			ActivationInfo,
			true,
			false);
	}
}

void UVacancyGameplayAbility::InputReleased(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	AVacancyPlayerCharacter* ReleasingPlayer = ResolvePlayer(ActorInfo);

	if (IsValid(ReleasingPlayer))
	{
		OnAbilityInputReleased(ReleasingPlayer);
	}

	if (InputBehavior == EVacancyAbilityInputBehavior::Hold && IsActive())
	{
		EndAbility(
			Handle,
			ActorInfo,
			ActivationInfo,
			true,
			false);
	}
}

bool UVacancyGameplayAbility::CanActivateVacancyAbility_Implementation(
	AVacancyPlayerCharacter* ActivatingPlayer) const
{
	return IsValid(ActivatingPlayer);
}

void UVacancyGameplayAbility::OnAbilityGranted_Implementation(
	AVacancyPlayerCharacter* GrantedPlayer)
{
}

void UVacancyGameplayAbility::OnAbilityActivated_Implementation(
	AVacancyPlayerCharacter* ActivatingPlayer)
{
}

void UVacancyGameplayAbility::OnAbilityEnded_Implementation(
	AVacancyPlayerCharacter* EndingPlayer,
	bool bWasCancelled)
{
}

void UVacancyGameplayAbility::OnAbilityCancelled_Implementation(
	AVacancyPlayerCharacter* CancellingPlayer)
{
}

void UVacancyGameplayAbility::OnAbilityInputPressed_Implementation(
	AVacancyPlayerCharacter* PressingPlayer)
{
}

void UVacancyGameplayAbility::OnAbilityInputReleased_Implementation(
	AVacancyPlayerCharacter* ReleasingPlayer)
{
}

FVacancyAbilityData UVacancyGameplayAbility::GetVacancyAbilityData() const
{
	const FVacancyAbilityData* AbilityData = GetVacancyAbilityDataPtr();
	return AbilityData != nullptr
		? *AbilityData
		: FVacancyAbilityData();
}

const FVacancyAbilityData*
UVacancyGameplayAbility::GetVacancyAbilityDataPtr() const
{
	if (!IsValid(VacancyAbilityDataAsset))
	{
		return nullptr;
	}

	return &VacancyAbilityDataAsset->GetAbilityDataRef();
}

AVacancyPlayerCharacter*
UVacancyGameplayAbility::GetVacancyPlayer() const
{
	return OwnerPlayer;
}

AVacancyPlayerCharacter* UVacancyGameplayAbility::ResolvePlayer(
	const FGameplayAbilityActorInfo* ActorInfo) const
{
	if (ActorInfo == nullptr)
	{
		return OwnerPlayer;
	}

	return Cast<AVacancyPlayerCharacter>(
		ActorInfo->AvatarActor.Get());
}

void UVacancyGameplayAbility::ApplyGrantedTags(
	const FGameplayAbilityActorInfo* ActorInfo)
{
	if (bGrantedTagsApplied
		|| ActorInfo == nullptr
		|| !ActorInfo->AbilitySystemComponent.IsValid())
	{
		return;
	}

	const FVacancyAbilityData* AbilityData = GetVacancyAbilityDataPtr();
	if (AbilityData == nullptr)
	{
		return;
	}

	const FGameplayTagContainer& GrantedTags =
		AbilityData->AbilityTagData.GrantedTags;

	if (GrantedTags.IsEmpty())
	{
		return;
	}

	ActorInfo->AbilitySystemComponent->AddLooseGameplayTags(GrantedTags);
	bGrantedTagsApplied = true;
}

void UVacancyGameplayAbility::RemoveGrantedTags(
	const FGameplayAbilityActorInfo* ActorInfo)
{
	if (!bGrantedTagsApplied)
	{
		return;
	}

	const FVacancyAbilityData* AbilityData = GetVacancyAbilityDataPtr();

	if (AbilityData != nullptr
		&& ActorInfo != nullptr
		&& ActorInfo->AbilitySystemComponent.IsValid())
	{
		ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTags(
			AbilityData->AbilityTagData.GrantedTags);
	}

	bGrantedTagsApplied = false;
}

bool UVacancyGameplayAbility::DebugAbility()
{
	return CVarDebugVacancyGameplayAbility.GetValueOnGameThread() > 0;
}