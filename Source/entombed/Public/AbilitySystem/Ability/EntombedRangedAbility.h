// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "EntombedDamageGameplayAbility.h"
#include "AbilitySystem/Ability/EntombedGameplayAbility.h"
#include "EntombedRangedAbility.generated.h"

struct FGameplayAbilityActivationInfo;
class AEntombedProjectile;
/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedRangedAbility : public UEntombedDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SetProjectileTarget(FVector Location);
	
	UFUNCTION(BlueprintCallable, Category="Projectile")
	bool SpawnProjectile(const FGameplayTag& SocketTag);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile");
	TSubclassOf<AEntombedProjectile> ProjectileClass;
	
	UPROPERTY(BlueprintReadOnly, Category="Projectile");
	FVector ProjectileTargetLocation = FVector();
};
