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

	GameplayTags.Attribute_Resource_LifeRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Derived.LifeRegeneration"), FString("How quickly Life regenerates."));
	GameplayTags.Attribute_Resource_FormRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Derived.FormRegeneration"), FString("How quickly Form regenerates."));
	GameplayTags.Attribute_Resource_MindRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Derived.MindRegeneration"), FString("How quickly Mind regenerates."));

	GameplayTags.Attribute_Derived_CriticalChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Derived.CriticalChance"), FString("Chance to do critical damage."));
	GameplayTags.Attribute_Derived_EvasionChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Derived.EvasionChance"), FString("Chance to do evade all damage."));

	/**
	*INPUT TAGS
	**/
	GameplayTags.Input_MainHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.MainHand"));
	GameplayTags.Input_OffHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.OffHand"));
	GameplayTags.Input_Ability_First = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Ability.First"));
	GameplayTags.Input_Ability_Second = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Ability.Second"));
	GameplayTags.Input_Ability_Third = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Ability.Third"));
	GameplayTags.Input_Ability_Fourth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Ability.Fourth"));
}
