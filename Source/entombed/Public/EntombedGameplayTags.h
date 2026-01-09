// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * EntombedGameplayTags
 *
 * singleton containing native gameplay tags
 */
struct FEntombedGameplayTags
{
public:
	static const FEntombedGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();
	
	/**
	* ATTRIBUTE TAGS
	**/
	FGameplayTag Attribute_TotalLife;
	FGameplayTag Attribute_LifeRegenerationRate;
	FGameplayTag Attribute_LifeRegenerationDelay;
	
	FGameplayTag Attribute_TotalForm;
	FGameplayTag Attribute_FormRegenerationRate;
	FGameplayTag Attribute_FormRegenerationDelay;
	
	FGameplayTag Attribute_CriticalChance;
	FGameplayTag Attribute_CriticalMultiplier;
	
	/**
	*INPUT TAGS
	**/
	FGameplayTag Input;
	FGameplayTag Input_Ability_MainHand_1;
	FGameplayTag Input_Ability_MainHand_2;
	FGameplayTag Input_Ability_OffHand_1;
	FGameplayTag Input_Ability_OffHand_2;
	FGameplayTag Input_Ability_OffHand_3;
	FGameplayTag Input_Ability_Archetype_1;
	FGameplayTag Input_Ability_Archetype_2;
	FGameplayTag Input_Ability_Archetype_3;

	/**
	*DAMAGE TAGS
	**/
	
	FGameplayTag Damage;

	FGameplayTag Damage_Sharp;
	FGameplayTag Damage_Point;
	FGameplayTag Damage_Blunt;
	
	FGameplayTag Damage_Burn;
	FGameplayTag Damage_Shock;
	FGameplayTag Damage_Freeze;
	
	TArray<FGameplayTag> DamageTypes;
	
	FGameplayTag Debuff;
	
	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Burn_Chance;
	FGameplayTag Debuff_Burn_Damage;
	FGameplayTag Debuff_Burn_Duration;
	FGameplayTag Debuff_Burn_Period;
	
	FGameplayTag Debuff_Shock;
	FGameplayTag Debuff_Shock_Chance;
	FGameplayTag Debuff_Shock_Damage;
	FGameplayTag Debuff_Shock_Duration;
	FGameplayTag Debuff_Shock_Period;
	
	FGameplayTag Debuff_Freeze;
	FGameplayTag Debuff_Freeze_Chance;
	FGameplayTag Debuff_Freeze_Damage;
	FGameplayTag Debuff_Freeze_Duration;
	FGameplayTag Debuff_Freeze_Period;

	FGameplayTag Resistance_Burn;
	FGameplayTag Resistance_Shock;
	FGameplayTag Resistance_Freeze;
	FGameplayTag Resistance_Elemental;

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffChances;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffDamages;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffDurations;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffPeriods;

	/**
	*ABILITY TAGS
	**/
	FGameplayTag Ability;
	FGameplayTag Ability_None;
	FGameplayTag Ability_Attack;
	FGameplayTag Ability_Ranged;
	FGameplayTag Ability_Summon;
	
	FGameplayTag Ability_Knock;

	FGameplayTag Ability_Status;
	FGameplayTag Ability_Status_Locked;
	FGameplayTag Ability_Status_Eligible;
	FGameplayTag Ability_Status_Unlocked;
	FGameplayTag Ability_Status_Equipped;

	FGameplayTag Ability_Type_Active;
	FGameplayTag Ability_Type_Passive;
	FGameplayTag Ability_Type_None;
	
	
	FGameplayTag Ability_FireBolt;
	FGameplayTag Cooldown_FireBolt;

	FGameplayTag Ability_Electrocute;
	FGameplayTag Cooldown_Electrocute;

	FGameplayTag Ability_ArcaneShards;
	FGameplayTag Cooldown_ArcaneShards;

	FGameplayTag Ability_FireBlast;
	FGameplayTag Cooldown_FireBlast;

	FGameplayTag Ability_Passive_Defense;
	FGameplayTag Ability_Passive_Siphon;
	FGameplayTag Ability_Passive_ListenForEvent;

	/**
	*EFFECT TAGS
	**/
	FGameplayTag Effect_Knockback;

	/**
	*CUE TAGS
	**/
	FGameplayTag GameplayCue_Impact;
	FGameplayTag GameplayCue_ShockBurst;
	FGameplayTag GameplayCue_ShockLoop;
	FGameplayTag GameplayCue_ArcaneShards;
	FGameplayTag GameplayCue_FireBlast;
	
	/**
	*SOCKET TAGS
	**/
	FGameplayTag Socket_MainHand;
	FGameplayTag Socket_OffHand;

	/**
	*MONTAGE TAGS
	**/
	FGameplayTag Montage_MainHand_Chop;
	FGameplayTag Montage_MainHand_Slice;
	FGameplayTag Montage_MainHand_Punch;
	FGameplayTag Montage_MainHand_Cast;
	FGameplayTag Montage_MainHand_Shoot;
	FGameplayTag Montage_MainHand_Throw;

	/**
	*MONTAGE EVENT TAGS
	**/
	FGameplayTag Event_Montage_Ability_Shoot;
	FGameplayTag Event_Montage_Ability_Melee;

	/**
	*META TAGS
	**/
	FGameplayTag Attribute_Meta_IncomingXP;
	
	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_CursorTrace;
	
private:
	static FEntombedGameplayTags GameplayTags;
};
