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

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDef;
	
	EntombedDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, CriticalChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, CriticalMultiplier, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, ArmorRating, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, BurnResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, ShockResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, FreezeResistance, Target, false);

		TagsToCaptureDef.Add(FEntombedGameplayTags::Get().Attribute_Derived_CriticalChance, CriticalChanceDef);
		TagsToCaptureDef.Add(FEntombedGameplayTags::Get().Attribute_Derived_CriticalMultiplier, CriticalMultiplierDef);
		TagsToCaptureDef.Add(FEntombedGameplayTags::Get().Attribute_Derived_BlockChance, BlockChanceDef);
		TagsToCaptureDef.Add(FEntombedGameplayTags::Get().Attribute_Derived_ArmorRating, ArmorRatingDef);
		TagsToCaptureDef.Add(FEntombedGameplayTags::Get().Resistance_Burn, BurnResistanceDef);
		TagsToCaptureDef.Add(FEntombedGameplayTags::Get().Resistance_Shock, ShockResistanceDef);
		TagsToCaptureDef.Add(FEntombedGameplayTags::Get().Resistance_Freeze, FreezeResistanceDef);
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

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceActor);
	
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetActor);

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
	for (auto& PhysicalDamageType : FEntombedGameplayTags::Get().PhysicalDamageTypes)
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
	const float DamageCoefficient = DamageCoefficientCurve->Eval(SourceCombatInterface->GetCharacterLevel());

	float DamageReductionFactor = FMath::Max(TargetArmorRating + DamageCoefficient * Damage, 1);
	float DamageReduction = TargetArmorRating / DamageReductionFactor; //TODO: change damage coefficient to something that scales better
	Damage = Damage * (1 - DamageReduction);

	//get elemental damage SetByCaller magnitudes
	for (auto& ElementalDamagePair : FEntombedGameplayTags::Get().ElementalDamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = ElementalDamagePair.Key;
		const FGameplayTag ResistanceTag = ElementalDamagePair.Value;
		
		checkf(EntombedDamageStatics().TagsToCaptureDef.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = EntombedDamageStatics().TagsToCaptureDef[ResistanceTag];

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
