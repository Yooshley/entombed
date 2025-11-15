// Copyright Yooshley


#include "EntombedGameplayTags.h"

#include "GameplayTagsManager.h"

FEntombedGameplayTags FEntombedGameplayTags::GameplayTags;

void FEntombedGameplayTags::InitializeNativeGameplayTags()
{
	/**
	*CORE ATTRIBUTE TAGS
	**/
	GameplayTags.Attribute_Core_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Core.Vigor"));
	GameplayTags.Attribute_Core_Instinct = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Core.Instinct"));
	GameplayTags.Attribute_Core_Technique = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Core.Technique"));
	GameplayTags.Attribute_Core_Acumen = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Core.Acumen"));
	GameplayTags.Attribute_Core_Logic = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Core.Logic"));
	GameplayTags.Attribute_Core_Spirit = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Core.Spirit"));
	
	/**
	*DERIVED ATTRIBUTE TAGS
	**/
	GameplayTags.Attribute_Resource_TotalLife = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Derived.TotalLife"), FString("Total Life."));
	GameplayTags.Attribute_Resource_TotalForm = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Derived.TotalForm"), FString("Total Form."));
	GameplayTags.Attribute_Resource_TotalForm = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Derived.TotalMind"), FString("Total Mind."));

	GameplayTags.Attribute_Resource_LifeRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Derived.LifeRegeneration"));
	GameplayTags.Attribute_Resource_FormRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Derived.FormRegeneration"));
	GameplayTags.Attribute_Resource_MindRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Derived.MindRegeneration"));

	GameplayTags.Attribute_Derived_CriticalChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Derived.CriticalChance"));
	GameplayTags.Attribute_Derived_CriticalMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Derived.CriticalMultiplier"));
	GameplayTags.Attribute_Derived_EvadeChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Derived.EvadeChance"));
	GameplayTags.Attribute_Derived_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Derived.BlockChance"));
	GameplayTags.Attribute_Derived_ArmorRating = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Derived.ArmorRating"));

	/**
	*INPUT TAGS
	**/
	GameplayTags.Input = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input"));
	GameplayTags.Input_Ability_MainHand_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Ability.MainHand.1"));
	GameplayTags.Input_Ability_MainHand_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Ability.MainHand.2"));
	GameplayTags.Input_Ability_OffHand_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Ability.OffHand.1"));
	GameplayTags.Input_Ability_OffHand_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Ability.OffHand.2"));
	GameplayTags.Input_Ability_OffHand_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Ability.OffHand.3"));
	GameplayTags.Input_Ability_Archetype_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Ability.Archetype.1"));
	GameplayTags.Input_Ability_Archetype_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Ability.Archetype.2"));
	GameplayTags.Input_Ability_Archetype_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Ability.Archetype.3"));

	/**
	*DAMAGE TAGS
	**/
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"));
	
	GameplayTags.Damage_Sharp = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Sharp"));
	GameplayTags.Damage_Point = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Point"));
	GameplayTags.Damage_Blunt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Blunt"));

	GameplayTags.PhysicalDamageTypes.Add(GameplayTags.Damage_Sharp);
	GameplayTags.PhysicalDamageTypes.Add(GameplayTags.Damage_Point);
	GameplayTags.PhysicalDamageTypes.Add(GameplayTags.Damage_Blunt);
	
	GameplayTags.Damage_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Burn"));
	GameplayTags.Damage_Shock = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Shock"));
	GameplayTags.Damage_Freeze = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Freeze"));
	
	GameplayTags.Resistance_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Resistance.Burn"));
	GameplayTags.Resistance_Shock = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Resistance.Shock"));
	GameplayTags.Resistance_Freeze = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Resistance.Freeze"));
	
	GameplayTags.ElementalDamageTypesToResistances.Add(GameplayTags.Damage_Burn, GameplayTags.Resistance_Burn);
	GameplayTags.ElementalDamageTypesToResistances.Add(GameplayTags.Damage_Shock, GameplayTags.Resistance_Shock);
	GameplayTags.ElementalDamageTypesToResistances.Add(GameplayTags.Damage_Freeze, GameplayTags.Resistance_Freeze);

	/**
	*ABILITY TAGS
	**/
	GameplayTags.Ability = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability"));
	GameplayTags.Ability_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Attack"));
	GameplayTags.Ability_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Summon"));
	
	GameplayTags.Ability_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.FireBolt"));
	GameplayTags.Cooldown_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.FireBolt"));

	/**
	*EFFECT TAGS
	**/
	GameplayTags.Effect_Knockback = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effect.Knockback"));

	/**
	*EFFECT TAGS
	**/
	GameplayTags.GameplayCue_Impact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Impact"));

	/**
	*SOCKET TAGS
	**/
	GameplayTags.Socket_MainHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Socket.MainHand"));
	GameplayTags.Socket_OffHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Socket.OffHand"));

	/**
	*MONTAGE TAGS
	**/
	GameplayTags.Montage_MainHand_Chop = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.MainHand.Chop"));
	GameplayTags.Montage_MainHand_Slice = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.MainHand.Slice"));
	GameplayTags.Montage_MainHand_Punch = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.MainHand.Punch"));
	GameplayTags.Montage_MainHand_Cast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.MainHand.Cast"));
	GameplayTags.Montage_MainHand_Shoot = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.MainHand.Shoot"));
	GameplayTags.Montage_MainHand_Throw = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.MainHand.Throw"));

	/**
	*MONTAGE EVENT TAGS
	**/
	GameplayTags.Event_Montage_Ability_Shoot = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.Ability_Attack.Shoot"));
	GameplayTags.Event_Montage_Ability_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Montage.Ability_Attack.Melee"));
}
