// Copyright Yooshley


#include "AbilitySystem/ModMagCalc/MMC_MaxLife.h"

#include "AbilitySystem/EntombedAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxLife::UMMC_MaxLife()
{
	VigorDefinition.AttributeToCapture = UEntombedAttributeSet::GetVigorAttribute();
	VigorDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDefinition);
}

float UMMC_MaxLife::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0;
	GetCapturedAttributeMagnitude(VigorDefinition, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.f);

	// gather non-attribute values to include in calculation(for reference)
	//ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	//const int32 Level = CombatInterface->GetCharacterLevel();

	//TODO: define base max life value for each class, use that value
	constexpr int32 BaseMaxLife = 50.f;
	return BaseMaxLife + (Vigor * 10.f);
}
