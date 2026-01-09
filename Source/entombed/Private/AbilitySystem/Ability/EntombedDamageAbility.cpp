// Copyright Yooshley


#include "AbilitySystem/Ability/EntombedDamageAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

UEntombedDamageAbility::UEntombedDamageAbility()
{

}

FAbilityDamageParameters UEntombedDamageAbility::GetDefaultDamageParameters(AActor* TargetActor) const
{
	FAbilityDamageParameters DamageEffectParameters;
	DamageEffectParameters.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if(TargetActor) DamageEffectParameters.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	DamageEffectParameters.AbilityLevel = GetAbilityLevel();
	DamageEffectParameters.DamageEffectClass =  AbilityDamageParameters.DamageEffectClass;
	DamageEffectParameters.DamageTypes = AbilityDamageParameters.DamageTypes;
	return DamageEffectParameters;
}
