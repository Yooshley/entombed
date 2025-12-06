// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/EntombedDamageGameplayAbility.h"
#include "EntombedFireBlast.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedFireBlast : public UEntombedDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual FString GetLevelDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	int32 NumFireBalls = 12;
};
