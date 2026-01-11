// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "EntombedAbilityTypes.h"
#include "AbilitySystem/Ability/EntombedGameplayAbility.h"
#include "EntombedProjectileAbility.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedProjectileAbility : public UEntombedGameplayAbility
{
	GENERATED_BODY()

public:
	UEntombedProjectileAbility();
	
	UFUNCTION(BlueprintPure)
	FAbilityDamageParameters GetDamageParameters(AActor* TargetActor = nullptr) const;
	
	UFUNCTION(BlueprintPure)
	FAbilityProjectileParameters GetProjectileParameters(AActor* TargetActor = nullptr) const;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SetTarget(FVector Location);
	
	UFUNCTION(BlueprintCallable, Category="Projectile")
	virtual void SpawnProjectile(const FGameplayTag& SocketTag);
	
	UPROPERTY(BlueprintReadOnly, Category="Projectile");
	FVector TargetLocation = FVector();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	FAbilityProjectileParameters AbilityProjectileParameters;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Damage")
	FAbilityDamageParameters AbilityDamageParameters;
};
