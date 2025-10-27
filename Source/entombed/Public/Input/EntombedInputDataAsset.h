// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "EntombedInputDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FEntombedInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedInputDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FEntombedInputAction> InputActions;
	
	const UInputAction* GetInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;
};
