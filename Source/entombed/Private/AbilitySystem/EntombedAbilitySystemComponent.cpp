// Copyright Yooshley


#include "AbilitySystem/EntombedAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EntombedGameplayTags.h"
#include "AbilitySystem/EntombedAbilitySystemLibrary.h"
#include "AbilitySystem/Ability/EntombedGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "entombed/EntombedLogChannels.h"
#include "Interaction/PlayerInterface.h"

struct FEntombedAbilityInfo;

void UEntombedAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UEntombedAbilitySystemComponent::ClientEffectApplied);
}

void UEntombedAbilitySystemComponent::AbilityInputHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

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

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
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

FGameplayTag UEntombedAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*AbilitySpec);
	}
	return FGameplayTag();
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
			ClearAbilitiesFromSlot(SlotTag);
			ClearSlot(AbilitySpec);
			AbilitySpec->GetDynamicSpecSourceTags().AddTag(SlotTag);

			if (StatusTag.MatchesTagExact(GameplayTags.Ability_Status_Unlocked))
			{
				AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(GameplayTags.Ability_Status_Unlocked);
				AbilitySpec->GetDynamicSpecSourceTags().AddTag(GameplayTags.Ability_Status_Equipped);
			}
			MarkAbilitySpecDirty(*AbilitySpec);
		}
		ClientEquipAbility(AbilityTag, GameplayTags.Ability_Status_Equipped, SlotTag, PrevSlotTag);
	}
}

void UEntombedAbilitySystemComponent::ClientEquipAbility(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& PrevSlotTag)
{
	AbilityEquippedDelegate.Broadcast(AbilityTag, StatusTag, SlotTag, PrevSlotTag);
}

bool UEntombedAbilitySystemComponent::GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag,
                                                                 FString& OutDescription, FString& OutNextLevelDescription)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (UEntombedGameplayAbility* GameplayAbility = Cast<UEntombedGameplayAbility>(AbilitySpec->Ability))
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
		OutDescription = UEntombedGameplayAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoByTag(AbilityTag).LevelRequirement);
	}
	OutNextLevelDescription = FString();
	return false;
}

void UEntombedAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* AbilitySpec)
{
	const FGameplayTag& SlotTag = GetInputTagFromSpec(*AbilitySpec);
	AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(SlotTag);
	MarkAbilitySpecDirty(*AbilitySpec);
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
