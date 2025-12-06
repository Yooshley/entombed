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
	*CORE ATTRIBUTE TAGS
	**/

	FGameplayTag Attribute_Core_Vigor;
	FGameplayTag Attribute_Core_Instinct;
	FGameplayTag Attribute_Core_Technique;
	FGameplayTag Attribute_Core_Acumen;
	FGameplayTag Attribute_Core_Logic;
	FGameplayTag Attribute_Core_Spirit;
	
	/**
	*DERIVED ATTRIBUTE TAGS
	**/
	FGameplayTag Attribute_Resource_TotalLife;
	FGameplayTag Attribute_Resource_TotalForm;
	FGameplayTag Attribute_Resource_TotalMind;

	FGameplayTag Attribute_Resource_LifeRegeneration;
	FGameplayTag Attribute_Resource_FormRegeneration;
	FGameplayTag Attribute_Resource_MindRegeneration;
	
	FGameplayTag Attribute_Derived_CriticalChance;
	FGameplayTag Attribute_Derived_CriticalMultiplier;
	FGameplayTag Attribute_Derived_EvadeChance;
	FGameplayTag Attribute_Derived_BlockChance;
	FGameplayTag Attribute_Derived_ArmorRating;

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

	TArray<FGameplayTag> PhysicalDamageTypes;
	
	FGameplayTag Damage_Burn;
	FGameplayTag Damage_Shock;
	FGameplayTag Damage_Freeze;
	
	FGameplayTag Debuff;
	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Frequency;
	
	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Shock;
	FGameplayTag Debuff_Freeze;

	FGameplayTag Resistance_Burn;
	FGameplayTag Resistance_Shock;
	FGameplayTag Resistance_Freeze;

	TMap<FGameplayTag, FGameplayTag> ElementalDamageTypesToResistances;
	TMap<FGameplayTag, FGameplayTag> ElementalDamageTypesToDebuffs;

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

	FGameplayTag Ability_Passive_Defense;
	FGameplayTag Ability_Passive_Siphon;

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
