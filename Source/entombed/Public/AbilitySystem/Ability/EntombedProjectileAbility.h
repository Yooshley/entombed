// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "EntombedDamageGameplayAbility.h"
#include "AbilitySystem/Ability/EntombedGameplayAbility.h"
#include "EntombedProjectileAbility.generated.h"

class AEntombedProjectile;
/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedProjectileAbility : public UEntombedDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="Projectile")
	bool SpawnProjectile(const FVector& ProjectileTargetLocation);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	TSubclassOf<AEntombedProjectile> ProjectileClass;
};
