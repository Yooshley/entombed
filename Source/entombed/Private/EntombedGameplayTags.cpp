// Copyright Yooshley


#include "EntombedGameplayTags.h"

#include "GameplayTagsManager.h"

FEntombedGameplayTags FEntombedGameplayTags::GameplayTags;

void FEntombedGameplayTags::InitializeNativeGameplayTags()
{
	/**
	*ATTRIBUTE TAGS
	**/
	GameplayTags.Attribute_TotalLife = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.TotalLife"));
	GameplayTags.Attribute_LifeRegenerationRate = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.LifeRegenerationRate"));
	GameplayTags.Attribute_LifeRegenerationDelay = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.LifeRegenerationDelay"));
	
	GameplayTags.Attribute_TotalForm = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.TotalForm"));
	GameplayTags.Attribute_FormRegenerationRate = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.FormRegenerationRate"));
	GameplayTags.Attribute_FormRegenerationDelay = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.FormRegenerationDelay"));

	GameplayTags.Attribute_CriticalChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.CriticalChance"));
	GameplayTags.Attribute_CriticalMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.CriticalMultiplier"));

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
	
	GameplayTags.Damage_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Burn"));
	GameplayTags.Damage_Shock = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Shock"));
	GameplayTags.Damage_Freeze = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Freeze"));
	
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Sharp);
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Point);
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Blunt);
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Burn);
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Shock);
	GameplayTags.DamageTypes.Add(GameplayTags.Damage_Freeze);
	
	GameplayTags.Debuff = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff"));
	
	GameplayTags.Debuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Burn"));
	GameplayTags.Debuff_Burn_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Burn.Chance"));
	GameplayTags.Debuff_Burn_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Burn.Damage"));
	GameplayTags.Debuff_Burn_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Burn.Duration"));
	GameplayTags.Debuff_Burn_Period = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Burn.Period"));
	
	GameplayTags.Debuff_Shock = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Shock"));
	GameplayTags.Debuff_Shock_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Shock.Chance"));
	GameplayTags.Debuff_Shock_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Shock.Damage"));
	GameplayTags.Debuff_Shock_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Shock.Duration"));
	GameplayTags.Debuff_Shock_Period = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Shock.Period"));
	
	GameplayTags.Debuff_Freeze = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Freeze"));
	GameplayTags.Debuff_Freeze_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Freeze.Chance"));
	GameplayTags.Debuff_Freeze_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Freeze.Damage"));
	GameplayTags.Debuff_Freeze_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Freeze.Duration"));
	GameplayTags.Debuff_Freeze_Period = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Freeze.Period"));
	
	GameplayTags.Resistance_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Resistance.Burn"));
	GameplayTags.Resistance_Shock = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Resistance.Shock"));
	GameplayTags.Resistance_Freeze = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Resistance.Freeze"));
	GameplayTags.Resistance_Elemental = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Resistance.Elemental"));
	
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Sharp, FGameplayTag());
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Point, FGameplayTag());
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Blunt, FGameplayTag());
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Burn, GameplayTags.Resistance_Burn);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Shock, GameplayTags.Resistance_Shock);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Freeze, GameplayTags.Resistance_Freeze);
	
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Burn, GameplayTags.Debuff_Burn);
	GameplayTags.DamageTypesToDebuffChances.Add(GameplayTags.Damage_Burn, GameplayTags.Debuff_Burn_Chance);
	GameplayTags.DamageTypesToDebuffDamages.Add(GameplayTags.Damage_Burn, GameplayTags.Debuff_Burn_Damage);
	GameplayTags.DamageTypesToDebuffDurations.Add(GameplayTags.Damage_Burn, GameplayTags.Debuff_Burn_Duration);
	GameplayTags.DamageTypesToDebuffPeriods.Add(GameplayTags.Damage_Burn, GameplayTags.Debuff_Burn_Period);
	
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Shock, GameplayTags.Debuff_Shock);
	GameplayTags.DamageTypesToDebuffChances.Add(GameplayTags.Damage_Shock, GameplayTags.Debuff_Shock_Chance);
	GameplayTags.DamageTypesToDebuffDamages.Add(GameplayTags.Damage_Shock, GameplayTags.Debuff_Shock_Damage);
	GameplayTags.DamageTypesToDebuffDurations.Add(GameplayTags.Damage_Shock, GameplayTags.Debuff_Shock_Duration);
	GameplayTags.DamageTypesToDebuffPeriods.Add(GameplayTags.Damage_Shock, GameplayTags.Debuff_Shock_Period);
	
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Freeze, GameplayTags.Debuff_Freeze);
	GameplayTags.DamageTypesToDebuffChances.Add(GameplayTags.Damage_Freeze, GameplayTags.Debuff_Freeze_Chance);
	GameplayTags.DamageTypesToDebuffDamages.Add(GameplayTags.Damage_Freeze, GameplayTags.Debuff_Freeze_Damage);
	GameplayTags.DamageTypesToDebuffDurations.Add(GameplayTags.Damage_Freeze, GameplayTags.Debuff_Freeze_Duration);
	GameplayTags.DamageTypesToDebuffPeriods.Add(GameplayTags.Damage_Freeze, GameplayTags.Debuff_Freeze_Period);

	/**
	*ABILITY TAGS
	**/
	GameplayTags.Ability = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability"));
	GameplayTags.Ability_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.None"));
	GameplayTags.Ability_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Attack"));
	GameplayTags.Ability_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Summon"));

	GameplayTags.Ability_Knock = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Knock"));

	GameplayTags.Ability_Status = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Status"));
	GameplayTags.Ability_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Status.Locked"));
	GameplayTags.Ability_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Status.Eligible"));
	GameplayTags.Ability_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Status.Unlocked"));
	GameplayTags.Ability_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Status.Equipped"));

	GameplayTags.Ability_Type_Active = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Type.Active"));
	GameplayTags.Ability_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Type.Passive"));
	GameplayTags.Ability_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Type.None"));
	
	GameplayTags.Ability_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.FireBolt"));
	GameplayTags.Cooldown_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.FireBolt"));
	
	GameplayTags.Ability_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Electrocute"));
	GameplayTags.Cooldown_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Electrocute"));
	
	GameplayTags.Ability_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.ArcaneShards"));
	GameplayTags.Cooldown_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.ArcaneShards"));

	GameplayTags.Ability_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.FireBlast"));
	GameplayTags.Cooldown_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.FireBlast"));

	GameplayTags.Ability_Passive_Defense = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Passive.Defense"));
	GameplayTags.Ability_Passive_Siphon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Passive.Siphon"));
	GameplayTags.Ability_Passive_ListenForEvent = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Passive.ListenForEvent"));

	/**
	*EFFECT TAGS
	**/
	GameplayTags.Effect_Knockback = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effect.Knockback"));

	/**
	*CUE TAGS
	**/
	GameplayTags.GameplayCue_Impact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Impact"));
	GameplayTags.GameplayCue_ShockBurst = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.ShockBurst"));
	GameplayTags.GameplayCue_ShockLoop = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.ShockLoop"));
	GameplayTags.GameplayCue_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.ArcaneShards"));
	GameplayTags.GameplayCue_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.FireBlast"));

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

	/**
	*META TAGS
	**/
	GameplayTags.Attribute_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Meta.IncomingXP"));

	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputPressed"));
	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputHeld"));
	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputReleased"));
	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.CursorTrace"));
}
