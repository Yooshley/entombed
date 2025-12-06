// Copyright Yooshley


#include "AbilitySystem/Ability/EntombedDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EntombedGameplayTags.h"

void UEntombedDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1);
	// for (TTuple<FGameplayTag, FScalableFloat> Pair : DamageTypes)
	// {
	// 	const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
	// 	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);
	// }
	const float ScaledDamage = DamageValue.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FDamageEffectParameters UEntombedDamageGameplayAbility::MakeDamageParametersFromClassDefaults(AActor* TargetActor) const
{
	FDamageEffectParameters DamageEffectParameters;
	DamageEffectParameters.WorldContextObject = GetAvatarActorFromActorInfo();
	DamageEffectParameters.DamageEffectClass = DamageEffectClass;
	DamageEffectParameters.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	DamageEffectParameters.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	DamageEffectParameters.AbilityLevel = GetAbilityLevel();
	DamageEffectParameters.DamageValue = DamageValue.GetValueAtLevel(GetAbilityLevel());
	DamageEffectParameters.DamageType = DamageType;
	DamageEffectParameters.DebuffChance = DebuffChance;
	DamageEffectParameters.DebuffDamage = DebuffDamage;
	DamageEffectParameters.DebuffDuration = DebuffDuration;
	DamageEffectParameters.DebuffFrequency = DebuffFrequency;
	DamageEffectParameters.DeathImpulseMagnitude = DeathImpulseMagnitude;
	DamageEffectParameters.KnockbackMagnitude = KnockbackMagnitude;

	if (bIsRadialDamage)
	{
		DamageEffectParameters.bIsRadialDamage = bIsRadialDamage;
		DamageEffectParameters.RadialDamageOrigin = RadialDamageOrigin;
		DamageEffectParameters.RadialDamageInnerRadius = RadialDamageInnerRadius;
		DamageEffectParameters.RadialDamageOuterRadius = RadialDamageOuterRadius;
	}

	if (IsValid(TargetActor))
	{
		FVector Origin = bIsRadialDamage
		? RadialDamageOrigin
		: GetAvatarActorFromActorInfo()->GetActorLocation();

		const FVector ToTarget = (TargetActor->GetActorLocation() - Origin).GetSafeNormal();

		DamageEffectParameters.DeathImpulse     = ToTarget * DeathImpulseMagnitude;
		DamageEffectParameters.KnockbackVector  = ToTarget * KnockbackMagnitude;
	}
	return DamageEffectParameters;
}

float UEntombedDamageGameplayAbility::GetDamageAtLevel() const
{
	return DamageValue.GetValueAtLevel(GetAbilityLevel());
}

// float UEntombedDamageGameplayAbility::GetDamageByTypeTag(float InLevel, const FGameplayTag& DamageType)
// {
// 	if (DamageTypes[DamageType].IsValid())
// 	{
// 		return DamageTypes[DamageType].GetValueAtLevel(InLevel);
// 	}
// 	return 0.0f;
// }
