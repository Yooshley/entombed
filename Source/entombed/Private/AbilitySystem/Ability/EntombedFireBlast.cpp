// Copyright Yooshley


#include "AbilitySystem/Ability/EntombedFireBlast.h"

FString UEntombedFireBlast::GetLevelDescription(int32 Level)
{
	const int32 ScaledDamage = DamageValue.GetValueAtLevel(Level);
	const float Cost = FMath::Abs(GetCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		"<Title>FIRE BLAST</>\n\n"

		"<Small>Level: </><Level>%d</>\n"
		"<Small>Cost: </><Cost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			
		"<Default>Launches %d balls of fire, exploding on impact and dealing </>"
		"<Damage>%d Damage.</>"), NumFireBalls, Cost, Cooldown, Level, ScaledDamage);
}

FString UEntombedFireBlast::GetNextLevelDescription(int32 Level)
{
	return GetLevelDescription(Level);
}
