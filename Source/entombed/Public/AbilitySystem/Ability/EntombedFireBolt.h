// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/EntombedRangedAbility.h"
#include "EntombedFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedFireBolt : public UEntombedRangedAbility
{
	GENERATED_BODY()
	
public:
	virtual FString GetLevelDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
