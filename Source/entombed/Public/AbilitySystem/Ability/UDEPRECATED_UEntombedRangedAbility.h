// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "EntombedProjectileAbility.h"
#include "AbilitySystem/Ability/EntombedAbility.h"
#include "UDEPRECATED_UEntombedRangedAbility.generated.h"

struct FGameplayAbilityActivationInfo;
class AEntombedAbilityProjectile;
/**
 * 
 */
UCLASS(Deprecated)
class ENTOMBED_API UDEPRECATED_UEntombedRangedAbility : public UEntombedProjectileAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SetProjectileTarget(FVector Location);

	virtual void SpawnProjectile(const FGameplayTag& SocketTag) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile");
	TSubclassOf<AEntombedAbilityProjectile> ProjectileClass;
	
	UPROPERTY(BlueprintReadOnly, Category="Projectile");
	FVector ProjectileTargetLocation = FVector();
};
