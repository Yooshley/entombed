// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "EntombedAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&)

/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;

	void AddDefaultAbilities(const TArray<TSubclassOf<UGameplayAbility>>& DefaultAbilities);

	void AbilityInputHeld(const FGameplayTag& InputTag);
	void AbilityInputReleased(const FGameplayTag& InputTag);
	
protected:
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
