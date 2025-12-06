// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "EntombedAbilityTypes.h"
#include "AbilitySystem/Ability/EntombedGameplayAbility.h"
#include "EntombedDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedDamageGameplayAbility : public UEntombedGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	FDamageEffectParameters MakeDamageParametersFromClassDefaults(AActor* TargetActor = nullptr) const;
	
	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	//UPROPERTY(EditDefaultsOnly, Category="Damage")
	//TMap<FGameplayTag, FScalableFloat> DamageTypes;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat DamageValue;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffFrequency = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffDuration = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DeathImpulseMagnitude = 5000.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float KnockbackMagnitude = 500.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	bool bIsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	FVector RadialDamageOrigin = FVector::ZeroVector;

	//float GetDamageByTypeTag(float InLevel, const FGameplayTag& DamageType);
};
