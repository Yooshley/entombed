// Copyright Yooshley


#include "AbilitySystem/EntombedAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UEntombedAttributeSet::UEntombedAttributeSet()
{
	InitLife(100.f);
	InitMaxLife(100.f);
	InitMana(100.f);
	InitMaxMana(100.f);
}

void UEntombedAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, Life, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, MaxLife, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UEntombedAttributeSet::OnRep_Life(const FGameplayAttributeData& PreviousLife) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, Life, PreviousLife);
}

void UEntombedAttributeSet::OnRep_MaxLife(const FGameplayAttributeData& PreviousMaxLife) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, MaxLife, PreviousMaxLife);
}

void UEntombedAttributeSet::OnRep_Mana(const FGameplayAttributeData& PreviousMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, Mana, PreviousMana);
}

void UEntombedAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& PreviousMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, MaxMana, PreviousMaxMana);
}
