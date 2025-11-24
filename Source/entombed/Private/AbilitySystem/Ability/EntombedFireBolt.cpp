// Copyright Yooshley


#include "AbilitySystem/Ability/EntombedFireBolt.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EntombedGameplayTags.h"

FString UEntombedFireBolt::GetLevelDescription(int32 Level)
{
	//const int32 BurnDamage = GetDamageByTypeTag(Level, FEntombedGameplayTags::Get().Damage_Burn);
	//const int32 BluntDamage = GetDamageByTypeTag(Level, FEntombedGameplayTags::Get().Damage_Blunt);

	const int32 ScaledDamage = DamageValue.GetValueAtLevel(Level);
	const float Cost = FMath::Abs(GetCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n\n"
			
			"<Small>Level: </><Level>%d</>\n"
			"<Small>Cost: </><Cost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			
			"<Default>Launches a bolt of fire, exploding on impact and dealing </>"
			"<Damage>%d Damage.</>"), Level, Cost, Cooldown, ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n\n"

			"<Small>Level: </><Level>%d</>\n"
			"<Small>Cost: </><Cost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			
			"<Default>Launches %d bolt of fire, exploding on impact and dealing </>"
			"<Damage>%d Damage.</>"), Level, Cost, Cooldown, NumberOfProjectiles * Level, ScaledDamage);
	}
}

FString UEntombedFireBolt::GetNextLevelDescription(int32 Level)
{
	//const int32 BurnDamage = GetDamageByTypeTag(Level, FEntombedGameplayTags::Get().Damage_Burn);
	//const int32 BluntDamage = GetDamageByTypeTag(Level, FEntombedGameplayTags::Get().Damage_Blunt);

	const int32 ScaledDamage = DamageValue.GetValueAtLevel(Level);
	const float Cost = FMath::Abs(GetCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL</>\n\n"

		"<Small>Level: </><Level>%d</>\n"
		"<Small>Cost: </><Cost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			
		"<Default>Launches %d bolt of fire, exploding on impact and dealing </>"
		"<Damage>%d Damage.</>"), Level, Cost, Cooldown, NumberOfProjectiles * Level, ScaledDamage);
}