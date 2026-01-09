// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "EntombedAbilityTypes.h"
#include "AbilitySystem/Ability/EntombedGameplayAbility.h"
#include "EntombedDamageAbility.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedDamageAbility : public UEntombedGameplayAbility
{
	GENERATED_BODY()

public:
	UEntombedDamageAbility();
	
	UFUNCTION(BlueprintPure)
	FAbilityDamageParameters GetDefaultDamageParameters(AActor* TargetActor = nullptr) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	FAbilityDamageParameters AbilityDamageParameter;
};
