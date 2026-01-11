// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "EntombedProjectileAbility.h"
#include "AbilitySystem/Ability/EntombedGameplayAbility.h"
#include "EntombedRangedAbility.generated.h"

struct FGameplayAbilityActivationInfo;
class AEntombedAbilityProjectile;
/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedRangedAbility : public UEntombedProjectileAbility
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
