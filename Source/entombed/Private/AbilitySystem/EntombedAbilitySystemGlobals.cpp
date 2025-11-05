// Copyright Yooshley


#include "AbilitySystem/EntombedAbilitySystemGlobals.h"

#include "EntombedAbilityTypes.h"

FGameplayEffectContext* UEntombedAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FEntombedGameplayEffectContext();
}
