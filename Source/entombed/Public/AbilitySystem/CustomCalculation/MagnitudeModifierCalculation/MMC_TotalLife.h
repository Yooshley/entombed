// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_TotalLife.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API UMMC_TotalLife : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_TotalLife();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition TotalLifeDefinition;
};
