// Copyright Yooshley


#include "AbilitySystem/EntombedAbilitySystemComponent.h"

#include "EntombedGameplayTags.h"
#include "AbilitySystem/Ability/EntombedGameplayAbility.h"

void UEntombedAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UEntombedAbilitySystemComponent::EffectApplied);
}

void UEntombedAbilitySystemComponent::AddDefaultAbilities(const TArray<TSubclassOf<UGameplayAbility>>& DefaultAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : DefaultAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UEntombedGameplayAbility* EntombedAbility = Cast<UEntombedGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(EntombedAbility->StartupInputTag);
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

void UEntombedAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                    const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	
	EffectAssetTags.Broadcast(TagContainer);
}
