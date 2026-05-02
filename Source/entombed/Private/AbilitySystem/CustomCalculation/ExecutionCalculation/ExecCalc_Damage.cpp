// Copyright Yooshley


#include "AbilitySystem/CustomCalculation/ExecutionCalculation/ExecCalc_Damage.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EntombedAbilityTypes.h"
#include "EntombedGameplayTags.h"
#include "AbilitySystem/EntombedAbilitySystemLibrary.h"
#include "AbilitySystem/EntombedAttributeSet.h"
#include "AbilitySystem/Data/ArchetypeInfo.h"
#include "entombed/Public/Interface/CombatInterface.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

struct EntombedDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BurnResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ShockResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FreezeResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ElementalResistance);
	
	EntombedDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, CriticalChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, CriticalMultiplier, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, BurnResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, ShockResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, FreezeResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEntombedAttributeSet, ElementalResistance, Target, false);
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
	RelevantAttributesToCapture.Add(DamageStatics().BurnResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ShockResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().FreezeResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ElementalResistanceDef);
}

void UExecCalc_Damage::DetermineDebuff(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    const FGameplayEffectSpec& Spec,
    FAggregatorEvaluateParameters EvaluationParameters,
    const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToCaptureDef) const
{
    const FEntombedGameplayTags& GameplayTags = FEntombedGameplayTags::Get();

    for (const TTuple<FGameplayTag, FGameplayTag>& ElementalDebuffPair : GameplayTags.DamageTypesToDebuffs)
    {
        const FGameplayTag DamageTypeTag = ElementalDebuffPair.Key;
    	
        const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageTypeTag, false, -1.f);
        if (TypeDamage <= -0.5f)
        {
            continue;
        }
    	
    	//Debuff Chance
        const FGameplayTag* ChanceTag = GameplayTags.DamageTypesToDebuffChances.Find(DamageTypeTag);
        if (!ChanceTag)
        {
            UE_LOG(LogTemp, Warning,
                TEXT("Missing DebuffChance mapping for DamageType: %s"),
                *DamageTypeTag.ToString());
            continue;
        }

        const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(*ChanceTag, false, 0.f);

		//Resistances
        float TargetDebuffResistance = 0.f;
        const FGameplayTag* ResistanceTag = GameplayTags.DamageTypesToResistances.Find(DamageTypeTag);
        if (ResistanceTag)
        {
            if (const FGameplayEffectAttributeCaptureDefinition* CaptureDef = InTagsToCaptureDef.Find(*ResistanceTag))
            {
                ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
                    *CaptureDef,
                    EvaluationParameters,
                    TargetDebuffResistance
                );
            }
        }

        TargetDebuffResistance = FMath::Max(TargetDebuffResistance, 0.f);
        const float EffectiveDebuffChance = SourceDebuffChance * (100.f - TargetDebuffResistance) / 100.f;

    	if (FMath::RandRange(1, 100) > EffectiveDebuffChance) continue;
    	
        FAppliedDebuffData NewDebuff;
        NewDebuff.DebuffType = DamageTypeTag;

        //Debuff Damage
        if (const FGameplayTag* DamageTag = GameplayTags.DamageTypesToDebuffDamages.Find(DamageTypeTag))
        {
            NewDebuff.Damage = Spec.GetSetByCallerMagnitude(*DamageTag, false, 0.f);
        }

        //Debuff Duration
        if (const FGameplayTag* DurationTag = GameplayTags.DamageTypesToDebuffDurations.Find(DamageTypeTag))
        {
            NewDebuff.Duration = Spec.GetSetByCallerMagnitude(*DurationTag, false, 0.f);
        }

        //Debuff Period
        if (const FGameplayTag* PeriodTag = GameplayTags.DamageTypesToDebuffPeriods.Find(DamageTypeTag))
        {
            NewDebuff.Period = Spec.GetSetByCallerMagnitude(*PeriodTag, false, 1.f);
        }
        else
        {
            NewDebuff.Period = 1.f;
        }
        
        FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

        if (FEntombedGameplayEffectContext* EntombedContext = static_cast<FEntombedGameplayEffectContext*>(ContextHandle.Get()))
        {
            EntombedContext->AddDebuff(NewDebuff);
        }
    }
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FEntombedGameplayTags& GameplayTags = FEntombedGameplayTags::Get();

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDef;

	TagsToCaptureDef.Add(GameplayTags.Attribute_CriticalChance, DamageStatics().CriticalChanceDef);
	TagsToCaptureDef.Add(GameplayTags.Attribute_CriticalMultiplier, DamageStatics().CriticalMultiplierDef);
	TagsToCaptureDef.Add(GameplayTags.Resistance_Burn, DamageStatics().BurnResistanceDef);
	TagsToCaptureDef.Add(GameplayTags.Resistance_Shock, DamageStatics().ShockResistanceDef);
	TagsToCaptureDef.Add(GameplayTags.Resistance_Freeze, DamageStatics().FreezeResistanceDef);
	TagsToCaptureDef.Add(GameplayTags.Resistance_Elemental, DamageStatics().ElementalResistanceDef);
	
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

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
	
	//get debuff SetByCaller magnitudes
	DetermineDebuff(ExecutionParams, Spec, EvaluationParameters, TagsToCaptureDef);

	//get damage SetByCaller magnitudes
	for (const TTuple<FGameplayTag, FGameplayTag>& DamagePair : GameplayTags.DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = DamagePair.Key;
		const FGameplayTag ResistanceTag = DamagePair.Value;
		
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag, false); //DefaultIfNotFound=0
		
		if (ResistanceTag.IsValid())
		{
			checkf(TagsToCaptureDef.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
			const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDef[ResistanceTag];

			float Resistance = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
			Resistance = FMath::Clamp(Resistance, 0.f, 100.f);
			DamageTypeValue *= (100.f - Resistance)/100;
		}
		Damage += DamageTypeValue;
	}
	
	//knockback
	float KnockbackForce = Spec.GetSetByCallerMagnitude(GameplayTags.Effect_Knockback,false,0.f);
	if (KnockbackForce > 0.f)
	{
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(FEntombedGameplayTags::Get().Effect_Knockback);
		TargetASC->TryActivateAbilitiesByTag(TagContainer);
		
		const FVector SourceLocation = SourceActor->GetActorLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();

		FVector KnockbackDirection = (TargetLocation - SourceLocation);
		KnockbackDirection.Z = 0.f;
		KnockbackDirection.Normalize();

		const FVector KnockbackVector = (TargetLocation - SourceLocation).GetSafeNormal() * KnockbackForce;
		if (!KnockbackVector.IsNearlyZero(1.f))
		{
			ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor);
			TargetCharacter->LaunchCharacter(KnockbackVector, true, false);
		}
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
