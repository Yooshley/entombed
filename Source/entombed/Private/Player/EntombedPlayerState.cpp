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
	DOREPLIFETIME(AEntombedPlayerState, XP);
	DOREPLIFETIME(AEntombedPlayerState, AttributePoints);
	DOREPLIFETIME(AEntombedPlayerState, AbilityPoints);
}

void AEntombedPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void AEntombedPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void AEntombedPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AEntombedPlayerState::OnRep_AbilityPoints(int32 OldAbilityPoints)
{
	OnAbilityPointsChangedDelegate.Broadcast(AbilityPoints);
}

UAbilitySystemComponent* AEntombedPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEntombedPlayerState::AddXP(const int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AEntombedPlayerState::AddLevel(const int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AEntombedPlayerState::AddAttributePoints(int32 InPoints)
{
	AttributePoints += InPoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AEntombedPlayerState::AddAbilityPoints(int32 InPoints)
{
	AbilityPoints += InPoints;
	OnAbilityPointsChangedDelegate.Broadcast(AbilityPoints);
}

void AEntombedPlayerState::SetXP(const int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AEntombedPlayerState::SetLevel(const int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}
