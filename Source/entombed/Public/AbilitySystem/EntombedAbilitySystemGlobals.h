// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "EntombedAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
