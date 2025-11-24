// Copyright Yooshley


#include "AbilitySystem/CustomCalculation/ExecutionCalculation/ExecCalc_Damage.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EntombedAbilityTypes.h"
#include "EntombedGameplayTags.h"
#include "AbilitySystem/EntombedAbilitySystemLibrary.h"
#include "AbilitySystem/EntombedAttributeSet.h"
#include "AbilitySystem/Data/ArchetypeInfo.h"
#include "Interaction/CombatInterface.h"

struct EntombedDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorRating);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BurnResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ShockResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FreezeResistance);
	
	EntombedDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, CriticalChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, CriticalMultiplier, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, ArmorRating, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, BurnResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, ShockResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, FreezeResistance, Target, false);
	}
};

static const EntombedDamageStatics& DamageStatics()
{
	static EntombedDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().CriticalChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalMultiplierDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorRatingDef);
	RelevantAttributesToCapture.Add(DamageStatics().BurnResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ShockResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().FreezeResistanceDef);
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToCaptureDef) const
{
	const FEntombedGameplayTags& GameplayTags = FEntombedGameplayTags::Get();
	for (const TTuple<FGameplayTag, FGameplayTag>& ElementalDebuffPair : GameplayTags.ElementalDamageTypesToDebuffs)
	{
		const FGameplayTag DamageTypeTag = ElementalDebuffPair.Key;
		const FGameplayTag DebuffTag = ElementalDebuffPair.Value;
    	
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageTypeTag, false, -1.f);
		if (TypeDamage > -0.5f) //padding for floating point imprecision
		{
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance, false, -1.f);
    		
			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.ElementalDamageTypesToResistances[DamageTypeTag];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToCaptureDef[ResistanceTag], EvaluationParameters, TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);
			const float EffectiveDebuffChance = SourceDebuffChance * (100 - TargetDebuffResistance) / 100.f;
			const bool bDebuff = FMath::RandRange(1, 100) < EffectiveDebuffChance;
			if (bDebuff)
			{
    			FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
				UEntombedAbilitySystemLibrary::SetIsDebuffed(ContextHandle, true);
				UEntombedAbilitySystemLibrary::SetDamageType(ContextHandle, DamageTypeTag);

				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Damage, false, -1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Duration, false, -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Frequency, false, -1.f);

				UEntombedAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UEntombedAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
				UEntombedAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
			}
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FEntombedGameplayTags& GameplayTags = FEntombedGameplayTags::Get();

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDef;

	TagsToCaptureDef.Add(GameplayTags.Attribute_Derived_CriticalChance, DamageStatics().CriticalChanceDef);
	TagsToCaptureDef.Add(GameplayTags.Attribute_Derived_CriticalMultiplier, DamageStatics().CriticalMultiplierDef);
	TagsToCaptureDef.Add(GameplayTags.Attribute_Derived_BlockChance, DamageStatics().BlockChanceDef);
	TagsToCaptureDef.Add(GameplayTags.Attribute_Derived_ArmorRating, DamageStatics().ArmorRatingDef);
	TagsToCaptureDef.Add(GameplayTags.Resistance_Burn, DamageStatics().BurnResistanceDef);
	TagsToCaptureDef.Add(GameplayTags.Resistance_Shock, DamageStatics().ShockResistanceDef);
	TagsToCaptureDef.Add(GameplayTags.Resistance_Freeze, DamageStatics().FreezeResistanceDef);
	
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourceLevel = 1;
	if (SourceActor->Implements<UCombatInterface>())
	{
		SourceLevel = ICombatInterface::Execute_GetCharacterLevel(SourceActor);
	}

	int32 TargetLevel = 1;
	if (TargetActor->Implements<UCombatInterface>())
	{
		TargetLevel = ICombatInterface::Execute_GetCharacterLevel(TargetActor);
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	
	float Damage = 0.f;

	//calculate block chance
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

	const bool bBlockedHit = FMath::RandRange(0, 100) < TargetBlockChance;
	UEntombedAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlockedHit);
		
	Damage = bBlockedHit ? 0.f : Damage;
	if (bBlockedHit) //if hit blocked exit early: TODO: convert blocked hits to form damage
	{
		const FGameplayModifierEvaluatedData EvaluatedData(UEntombedAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
		OutExecutionOutput.AddOutputModifier(EvaluatedData);
		return;
	}
	
	//get physical damage SetByCaller magnitudes
	for (const FGameplayTag& PhysicalDamageType : GameplayTags.PhysicalDamageTypes)
	{
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(PhysicalDamageType, false); //DefaultIfNotFound=0
		Damage += DamageTypeValue;
	}

	//calculate armor effectiveness against physical damage
	float TargetArmorRating = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorRatingDef, EvaluationParameters, TargetArmorRating);
	TargetArmorRating = FMath::Max<float>(TargetArmorRating, 0.f);

	UArchetypeInfo* ArchetypeInfo = UEntombedAbilitySystemLibrary::GetArchetypeInfo(SourceActor);
	FRealCurve* DamageCoefficientCurve = ArchetypeInfo->DamageCalculationCoefficients->FindCurve(FName("DamageCoefficient"), FString()); //TODO: magic string
	const float DamageCoefficient = DamageCoefficientCurve->Eval(SourceLevel);

	float DamageReductionFactor = FMath::Max(TargetArmorRating + DamageCoefficient * Damage, 1);
	float DamageReduction = TargetArmorRating / DamageReductionFactor; //TODO: change damage coefficient to something that scales better
	Damage = Damage * (1 - DamageReduction);
	
	//get elemental debuff SetByCaller magnitudes
	DetermineDebuff(ExecutionParams, Spec, EvaluationParameters, TagsToCaptureDef);

	//get elemental damage SetByCaller magnitudes
	for (const TTuple<FGameplayTag, FGameplayTag>& ElementalDamagePair : GameplayTags.ElementalDamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = ElementalDamagePair.Key;
		const FGameplayTag ResistanceTag = ElementalDamagePair.Value;
		
		checkf(TagsToCaptureDef.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDef[ResistanceTag];

		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);
		
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(ElementalDamagePair.Key, false); //DefaultIfNotFound=0
		DamageTypeValue *= (100.f - Resistance)/100;
		Damage += DamageTypeValue;
	}

	//calculate critical hit chance
	float SourceCriticalChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalChanceDef, EvaluationParameters, SourceCriticalChance);
	SourceCriticalChance = FMath::Max<float>(SourceCriticalChance, 0.f);

	float SourceCriticalMultiplier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalMultiplierDef, EvaluationParameters, SourceCriticalMultiplier);
	SourceCriticalMultiplier = FMath::Max<float>(SourceCriticalMultiplier, 1.f);
	
	const bool bCriticalHit = FMath::RandRange(1, 100) < SourceCriticalChance;
	UEntombedAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	
	Damage = bCriticalHit ? SourceCriticalMultiplier * Damage : Damage;

	const FGameplayModifierEvaluatedData EvaluatedData(UEntombedAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
