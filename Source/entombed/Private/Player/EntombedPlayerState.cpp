// Copyright Yooshley


#include "Player/EntombedPlayerState.h"

#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "AbilitySystem/EntombedAttributeSet.h"

AEntombedPlayerState::AEntombedPlayerState()
{
	SetNetUpdateFrequency(100.f);
	
	AbilitySystemComponent = CreateDefaultSubobject<UEntombedAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UEntombedAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AEntombedPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
