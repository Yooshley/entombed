// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EntombedAIController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class ENTOMBED_API AEntombedAIController : public AAIController
{
	GENERATED_BODY()
public:
	AEntombedAIController();

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
