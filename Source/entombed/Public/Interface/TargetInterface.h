// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TargetInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ENTOMBED_API ITargetInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void HighlightActor();
	
	UFUNCTION(BlueprintNativeEvent)
	void UnHighlightActor();
	
	UFUNCTION(BlueprintNativeEvent)
	void SetMoveToLocation(FVector& OutDestination);
};
