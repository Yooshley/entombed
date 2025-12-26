// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EntombedGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FName PlayerStartTag = FName();
	
	UPROPERTY()
	FString SlotName = FString();
	
	UPROPERTY()
	int32 SlotIndex = 0;
};
