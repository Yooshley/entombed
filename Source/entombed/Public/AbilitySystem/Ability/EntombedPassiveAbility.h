// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/EntombedGameplayAbility.h"
#include "EntombedPassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedPassiveAbility : public UEntombedGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void RecieveDeactivate(const FGameplayTag& AbilityTag);
};
