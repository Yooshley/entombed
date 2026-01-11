// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/EntombedProjectileAbility.h"
#include "EntombedFireBlast.generated.h"

class AEntombedFireBall;
/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedFireBlast : public UEntombedProjectileAbility
{
	GENERATED_BODY()

public:
	virtual FString GetLevelDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	TArray<AEntombedFireBall*> SpawnFireBalls();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	int32 NumFireBalls = 12;

private:
	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	TSubclassOf<AEntombedFireBall> FireBallClass;
};
