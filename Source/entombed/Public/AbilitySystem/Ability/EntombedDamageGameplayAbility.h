// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/EntombedGameplayAbility.h"
#include "EntombedDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedDamageGameplayAbility : public UEntombedGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
