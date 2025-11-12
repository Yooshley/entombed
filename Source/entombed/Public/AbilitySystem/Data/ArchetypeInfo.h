// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ArchetypeInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EEntombedArchetype : uint8
{
	/* PLAYER ARCHETYPES */
	Templar,
	Outlaw,
	Alchemist,
	Barbarian,
	Engineer,
	Hexer,
	
	/* ENEMY ARCHETYPES */
	Minion_Unarmed,
	Minion_Melee,
	Minion_Ranged,
	Risen_Marauder,
	Risen_Stalker,
	Risen_Warlock,

	Necromancer,
};

USTRUCT(BlueprintType)
struct FEntombedArchetypeDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Archetype Defaults")
	bool bIsRanged = false;

	UPROPERTY(EditDefaultsOnly, Category="Archetype Defaults")
	TSubclassOf<UGameplayEffect> CoreAttributesEffect;

	UPROPERTY(EditDefaultsOnly, Category="Archetype Defaults")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};

/**
 * 
 */
UCLASS()
class ENTOMBED_API UArchetypeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Archetype Defaults")
	TMap<EEntombedArchetype, FEntombedArchetypeDefaultInfo> ArchetypeInformation;
	
	UPROPERTY(EditDefaultsOnly, Category="Shared Archetype Defaults")
	TSubclassOf<UGameplayEffect> DerivedAttributesEffect;

	UPROPERTY(EditDefaultsOnly, Category="Shared Archetype Defaults")
	TSubclassOf<UGameplayEffect> ResourceAttributesEffect;

	UPROPERTY(EditDefaultsOnly, Category="Shared Archetype Defaults")
	TArray<TSubclassOf<UGameplayAbility>> SharedAbilities;

	UPROPERTY(EditDefaultsOnly, Category="Shared Archetype Defaults")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;

	FEntombedArchetypeDefaultInfo GetArchetypeDefaultInfo(EEntombedArchetype Archetype);
};
