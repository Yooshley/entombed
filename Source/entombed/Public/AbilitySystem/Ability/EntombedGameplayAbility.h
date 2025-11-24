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

	virtual FString GetLevelDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);
	
protected:
	float GetCost(float InLevel = 1.f) const;
	float GetCooldown(float InLevel = 1.f);
};
