// Copyright Yooshley


#include "AbilitySystem/CustomCalculation/MagnitudeModifierCalculation/MMC_TotalLife.h"

#include "AbilitySystem/EntombedAttributeSet.h"
//#include "Interaction/CombatInterface.h"

UMMC_TotalLife::UMMC_TotalLife()
{
	TotalLifeDefinition.AttributeToCapture = UEntombedAttributeSet::GetTotalLifeAttribute();
	TotalLifeDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	TotalLifeDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(TotalLifeDefinition);
}

float UMMC_TotalLife::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float TotalLife = 0;
	GetCapturedAttributeMagnitude(TotalLifeDefinition, Spec, EvaluationParameters, TotalLife);
	TotalLife = FMath::Max<float>(TotalLife, 0.f);
	
	return TotalLife;
}
