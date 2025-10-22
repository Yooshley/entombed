// Copyright Yooshley


#include "Player/EntombedPlayerState.h"

#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "AbilitySystem/EntombedAttributeSet.h"
#include "Net/UnrealNetwork.h"

AEntombedPlayerState::AEntombedPlayerState()
{
	SetNetUpdateFrequency(100.f);
	
	AbilitySystemComponent = CreateDefaultSubobject<UEntombedAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UEntombedAttributeSet>("AttributeSet");
}

void AEntombedPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEntombedPlayerState, Level);
}

void AEntombedPlayerState::OnRep_Level(int32 OldLevel)
{
}

UAbilitySystemComponent* AEntombedPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
