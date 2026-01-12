// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/UDEPRECATED_UEntombedRangedAbility.h"
#include "DEPRECATED_UEntombedFireBolt.generated.h"

/**
 * 
 */
UCLASS(Deprecated)
class ENTOMBED_API UDEPRECATED_UEntombedFireBolt : public UDEPRECATED_UEntombedRangedAbility
{
	GENERATED_BODY()
	
public:
	virtual FString GetLevelDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	
	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectiles(const FGameplayTag& SocketTag, AActor* HomingTarget = nullptr, bool bPitchOverride = false, float PitchOverride = 0.f);

protected:
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float ProjectileSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	int32 ProjectileCount = 5;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	bool bHomingProjectile = true;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float HomingAccelerationMin = 1500.f;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float HomingAccelerationMax = 3000.f;
};
