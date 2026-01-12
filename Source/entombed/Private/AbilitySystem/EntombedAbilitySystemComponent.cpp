// Copyright Yooshley


#include "AbilitySystem/EntombedAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EntombedGameplayTags.h"
#include "AbilitySystem/EntombedAbilitySystemLibrary.h"
#include "AbilitySystem/Ability/EntombedAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "entombed/EntombedLogChannels.h"
#include "entombed/Public/Interface/PlayerInterface.h"
#include "Game/EntombedSaveGame.h"

struct FSavedAbility;
struct FEntombedAbilityInfo;

void UEntombedAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UEntombedAbilitySystemComponent::ClientEffectApplied);
}

void UEntombedAbilitySystemComponent::GrantAbilitiesFromSaveData(UEntombedSaveGame* SaveData)
{
	for (const FSavedAbility& AbilityData : SaveData->SavedAbilities)
	{
		const TSubclassOf<UGameplayAbility> AbilityClass = AbilityData.GameplayAbility;
		
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, AbilityData.AbilityLevel);
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityData.AbilitySlot);
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityData.AbilityStatus);
		if (AbilityData.AbilityType == FEntombedGameplayTags::Get().Ability_Type_Active)
		{
			GiveAbility(AbilitySpec);
		}
		else if (AbilityData.AbilityType == FEntombedGameplayTags::Get().Ability_Type_Passive)
		{
			if (AbilityData.AbilityStatus.MatchesTagExact(FEntombedGameplayTags::Get().Ability_Status_Equipped))
			{
				GiveAbilityAndActivateOnce(AbilitySpec);
			}
			else
			{
				GiveAbility(AbilitySpec);
			}
		}
	}
	bGrantedDefaultAbilities = true;
	GrantedAbilitiesDelegate.Broadcast();
}

void UEntombedAbilitySystemComponent::AbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.GetAbilityInstances().Last()->GetCurrentActivationInfoRef().GetActivationPredictionKey());
			}
		}
	}
}

void UEntombedAbilitySystemComponent::AbilityInputHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
		
	}
}

void UEntombedAbilitySystemComponent::AbilityInputReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopeLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.GetAbilityInstances().Last()->GetCurrentActivationInfoRef().GetActivationPredictionKey());
		}
		
	}
}

void UEntombedAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this); //lock the current activatable abilities
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogEntombed, Error, TEXT("Failed to execute Delegate in [%hs]"), __FUNCTION__);
		}
	}
}

FGameplayTag UEntombedAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag AbilityTag : AbilitySpec.Ability.Get()->GetAssetTags())
		{
			if (AbilityTag.MatchesTag(FGameplayTag::RequestGameplayTag(FEntombedGameplayTags::Get().Ability.GetTagName())))
			{
				return AbilityTag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UEntombedAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag AbilityTag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (AbilityTag.MatchesTag(FGameplayTag::RequestGameplayTag(FEntombedGameplayTags::Get().Input.GetTagName())))
		{
			return AbilityTag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UEntombedAbilitySystemComponent::GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag AbilityTag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (AbilityTag.MatchesTag(FGameplayTag::RequestGameplayTag(FEntombedGameplayTags::Get().Ability_Status.GetTagName())))
		{
			return AbilityTag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UEntombedAbilitySystemComponent::GetStatusTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusTagFromSpec(*AbilitySpec);
	}
	return FGameplayTag();
}

FGameplayTag UEntombedAbilitySystemComponent::GetSlotTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*AbilitySpec);
	}
	return FGameplayTag();
}

bool UEntombedAbilitySystemComponent::SlotIsEmpty(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(AbilitySpec, Slot))
		{
			return false;
		}
	}
	return true;
}

bool UEntombedAbilitySystemComponent::AbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	return Spec.GetDynamicSpecSourceTags().HasTagExact(Slot);
}

bool UEntombedAbilitySystemComponent::AbilityHasAnySlot(const FGameplayAbilitySpec& Spec)
{
	return Spec.GetDynamicSpecSourceTags().HasTag(FGameplayTag::RequestGameplayTag(FName("Input")));
}

FGameplayAbilitySpec* UEntombedAbilitySystemComponent::GetSpecWithSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(Slot))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

bool UEntombedAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec& Spec) const
{
	UAbilityInfo* AbilityInfo = UEntombedAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	const FGameplayTag AbilityTag = GetAbilityTagFromSpec(Spec);
	const FEntombedAbilityInfo& Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
	const FGameplayTag AbilityTypeTag = Info.AbilityTag;
	return AbilityTypeTag.MatchesTagExact(FEntombedGameplayTags::Get().Ability_Type_Passive);
}

void UEntombedAbilitySystemComponent::AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	ClearSlot(&Spec);
	Spec.GetDynamicSpecSourceTags().AddTag(Slot);
}

void UEntombedAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& AbilityTag,
	bool bActivate)
{
	ActivatePassiveEffectDelegate.Broadcast(AbilityTag, bActivate);
}

FGameplayAbilitySpec* UEntombedAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->GetAssetTags())
		{
			if (Tag.MatchesTagExact(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

void UEntombedAbilitySystemComponent::UpdateAbilityStatus(int32 Level)
{
	UAbilityInfo* AbilityInfo = UEntombedAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FEntombedAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (Level < Info.LevelRequirement) continue;
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.AbilityClass, 1);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FEntombedGameplayTags::Get().Ability_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec); //mark for replication
			ClientUpdateAbilityStatus(Info.AbilityTag, FEntombedGameplayTags::Get().Ability_Status_Eligible, AbilitySpec.Level);
		}
	}
}

void UEntombedAbilitySystemComponent::ServerSpendAbilityPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddAbilityPoints(GetAvatarActor(), -1);
		}
		
		const FEntombedGameplayTags GameplayTags = FEntombedGameplayTags::Get();
		FGameplayTag StatusTag = GetStatusTagFromSpec(*AbilitySpec);
		if (StatusTag.MatchesTagExact(GameplayTags.Ability_Status_Eligible))
		{
			AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(GameplayTags.Ability_Status_Eligible);
			AbilitySpec->GetDynamicSpecSourceTags().AddTag(GameplayTags.Ability_Status_Unlocked);
			StatusTag = GameplayTags.Ability_Status_Unlocked;
		}
		else if (StatusTag.MatchesTagExact(GameplayTags.Ability_Status_Equipped) || StatusTag.MatchesTagExact(GameplayTags.Ability_Status_Unlocked))
		{
			AbilitySpec->Level += 1;
		}
		ClientUpdateAbilityStatus(AbilityTag, StatusTag, AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UEntombedAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& SlotTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FEntombedGameplayTags& GameplayTags = FEntombedGameplayTags::Get();
		const FGameplayTag& PrevSlotTag = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& StatusTag = GetStatusTagFromSpec(*AbilitySpec);
		
		if (StatusTag == GameplayTags.Ability_Status_Equipped || StatusTag == GameplayTags.Ability_Status_Unlocked)
		{
			if (!SlotIsEmpty(SlotTag)) // There is an ability in this slot already. Deactivate and clear its slot.
			{
				FGameplayAbilitySpec* SpecWithSlot = GetSpecWithSlot(SlotTag);
				if (SpecWithSlot)
				{
					// is that ability the same as this ability? If so, we can return early.
					if (AbilityTag.MatchesTagExact(GetAbilityTagFromSpec(*SpecWithSlot)))
					{
						ClientEquipAbility(AbilityTag, GameplayTags.Ability_Status_Equipped, SlotTag, PrevSlotTag);
						return;
					}

					if (IsPassiveAbility(*SpecWithSlot))
					{
						MulticastActivatePassiveEffect(GetAbilityTagFromSpec(*SpecWithSlot), false);
						DeactivatePassiveAbilityDelegate.Broadcast(GetAbilityTagFromSpec(*SpecWithSlot));
					}

					ClearSlot(SpecWithSlot);
				}
			}

			if (!AbilityHasAnySlot(*AbilitySpec)) // Ability doesn't yet have a slot (it's not active)
			{
				if (IsPassiveAbility(*AbilitySpec))
				{
					TryActivateAbility(AbilitySpec->Handle);
					MulticastActivatePassiveEffect(AbilityTag, true);
				}
				AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(GetStatusTagFromSpec(*AbilitySpec));
				AbilitySpec->GetDynamicSpecSourceTags().AddTag(FEntombedGameplayTags::Get().Ability_Status_Equipped);
			}
			AssignSlotToAbility(*AbilitySpec, SlotTag);
			MarkAbilitySpecDirty(*AbilitySpec);
		}
		ClientEquipAbility(AbilityTag, GameplayTags.Ability_Status_Equipped, SlotTag, PrevSlotTag);
	}
}

void UEntombedAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& PrevSlotTag)
{
	AbilityEquippedDelegate.Broadcast(AbilityTag, StatusTag, SlotTag, PrevSlotTag);
}

bool UEntombedAbilitySystemComponent::GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag,
                                                                 FString& OutDescription, FString& OutNextLevelDescription)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (UEntombedAbility* GameplayAbility = Cast<UEntombedAbility>(AbilitySpec->Ability))
		{
			OutDescription = GameplayAbility->GetLevelDescription(AbilitySpec->Level);
			OutNextLevelDescription = GameplayAbility->GetNextLevelDescription(AbilitySpec->Level + 1);
			return true;
		}
	}
	UAbilityInfo* AbilityInfo = UEntombedAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FEntombedGameplayTags::Get().Ability_None))
	{
		OutDescription = FString();
	}
	else
	{
		OutDescription = UEntombedAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoByTag(AbilityTag).LevelRequirement);
	}
	OutNextLevelDescription = FString();
	return false;
}

void UEntombedAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* AbilitySpec)
{
	const FGameplayTag& SlotTag = GetInputTagFromSpec(*AbilitySpec);
	AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(SlotTag);
}

void UEntombedAbilitySystemComponent::ClearAbilitiesFromSlot(const FGameplayTag& SlotTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if(IsAbilitySlotted(&Spec, SlotTag))
		{
			ClearSlot(&Spec);
		}
	}
}

bool UEntombedAbilitySystemComponent::IsAbilitySlotted(FGameplayAbilitySpec* Spec, const FGameplayTag& SlotTag)
{
	for (FGameplayTag Tag : Spec->GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTagExact(SlotTag))
		{
			return true;
		}
	}
	return false;
}

void UEntombedAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	OnGrantedAbilities();
}

void UEntombedAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	AbilityStatusChangedDelegate.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

void UEntombedAbilitySystemComponent::OnGrantedAbilities()
{
	if (!bGrantedDefaultAbilities)
	{
		bGrantedDefaultAbilities = true;
		GrantedAbilitiesDelegate.Broadcast();
	}
}

void UEntombedAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void UEntombedAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddAttributePoints(GetAvatarActor(), -1);
	}
}

void UEntombedAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                         const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	
	EffectAssetTags.Broadcast(TagContainer);
}
