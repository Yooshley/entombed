// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ENTOMBED_API ICombatInterface
{
	GENERATED_BODY()
public:
	virtual int32 GetCharacterLevel();
	virtual FVector GetMainHandSocketLocation();
	virtual FVector GetOffHandSocketLocation();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	virtual void Death() = 0;
};
