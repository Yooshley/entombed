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
	FGameplayTag Attribute_Derived_EvasionChance;

	/**
	*INPUT TAGS
	**/
	FGameplayTag Input_MainHand;
	FGameplayTag Input_OffHand;
	FGameplayTag Input_Ability_First;
	FGameplayTag Input_Ability_Second;
	FGameplayTag Input_Ability_Third;
	FGameplayTag Input_Ability_Fourth;
	
private:
	static FEntombedGameplayTags GameplayTags;
};
