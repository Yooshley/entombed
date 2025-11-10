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
	FGameplayTag Input_MainHand;
	FGameplayTag Input_OffHand;
	FGameplayTag Input_Ability_First;
	FGameplayTag Input_Ability_Second;
	FGameplayTag Input_Ability_Third;
	FGameplayTag Input_Ability_Fourth;

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

	FGameplayTag Resistance_Burn;
	FGameplayTag Resistance_Shock;
	FGameplayTag Resistance_Freeze;

	TMap<FGameplayTag, FGameplayTag> ElementalDamageTypesToResistances;

	/**
	*ABILITY TAGS
	**/
	FGameplayTag Ability;

	/**
	*EFFECT TAGS
	**/
	FGameplayTag Effect_Knockback;

	/**
	*CUE TAGS
	**/
	FGameplayTag GameplayCue_Impact;
	
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
	
private:
	static FEntombedGameplayTags GameplayTags;
};
