// Copyright Yooshley


#include "AbilitySystem/EntombedAbilitySystemComponent.h"

#include "EntombedGameplayTags.h"
#include "AbilitySystem/Ability/EntombedGameplayAbility.h"
#include "entombed/EntombedLogChannels.h"

void UEntombedAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UEntombedAbilitySystemComponent::ClientEffectApplied);
}

void UEntombedAbilitySystemComponent::AddDefaultAbilities(const TArray<TSubclassOf<UGameplayAbility>>& DefaultAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : DefaultAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UEntombedGameplayAbility* EntombedAbility = Cast<UEntombedGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(EntombedAbility->AbilityInputTag);
			GiveAbility(AbilitySpec);
		}
	}
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

void UEntombedAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	OnGrantedAbilities();
}

void UEntombedAbilitySystemComponent::OnGrantedAbilities()
{
	if (!bGrantedDefaultAbilities)
	{
		bGrantedDefaultAbilities = true;
		GrantedAbilitiesDelegate.Broadcast(this);
	}
}

void UEntombedAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                         const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	
	EffectAssetTags.Broadcast(TagContainer);
}
