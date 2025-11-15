// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelInfo.generated.h"

USTRUCT()
struct FEntombedLevelInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 XPRequirement = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 AttributeAward = 1;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityAward = 1;
};

/**
 * 
 */
UCLASS()
class ENTOMBED_API ULevelInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FEntombedLevelInfo> LevelInformation;

	int32 FindLevelForXP(int32 XP) const;
};
