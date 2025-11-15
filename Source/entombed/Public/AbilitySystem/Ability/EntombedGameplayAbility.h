// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EntombedGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag AbilityInputTag;
};
