// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/EntombedProjectileAbility.h"
#include "DEPRECATED_UEntombedFireBlast.generated.h"

class ADEPRECATED_AEntombedFireBall;
/**
 * 
 */
UCLASS(Deprecated)
class ENTOMBED_API UDEPRECATED_UEntombedFireBlast : public UEntombedProjectileAbility
{
	GENERATED_BODY()

public:
	virtual FString GetLevelDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	
	UFUNCTION(BlueprintCallable, meta=(DeprecatedFunction))
	TArray<ADEPRECATED_AEntombedFireBall*> SpawnFireBalls();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	int32 NumFireBalls = 12;
};
