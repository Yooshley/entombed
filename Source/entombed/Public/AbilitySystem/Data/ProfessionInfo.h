// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProfessionInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EProfession : uint8
{
	Crusader,
	Saboteur,
	Sorcerer,
	Marauder,
	Mechanist,
	Cultist
};

USTRUCT(BlueprintType)
struct FProfessionDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Profession Defaults")
	TSubclassOf<UGameplayEffect> CoreAttributesEffect;
};

/**
 * 
 */
UCLASS()
class ENTOMBED_API UProfessionInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Profession Defaults")
	TMap<EProfession, FProfessionDefaultInfo> ProfessionInformation;
	
	UPROPERTY(EditDefaultsOnly, Category="Shared Profession Defaults")
	TSubclassOf<UGameplayEffect> DerivedAttributesEffect;

	UPROPERTY(EditDefaultsOnly, Category="Shared Profession Defaults")
	TSubclassOf<UGameplayEffect> ResourceAttributesEffect;

	UPROPERTY(EditDefaultsOnly, Category="Shared Profession Defaults")
	TArray<TSubclassOf<UGameplayAbility>> SharedAbilities;

	UPROPERTY(EditDefaultsOnly, Category="Shared Profession Defaults")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;

	FProfessionDefaultInfo GetProfessionDefaultInfo(EProfession Profession);
};
