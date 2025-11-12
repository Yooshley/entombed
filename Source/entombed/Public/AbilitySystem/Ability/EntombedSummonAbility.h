// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/EntombedGameplayAbility.h"
#include "EntombedSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedSummonAbility : public UEntombedGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();
	
	UFUNCTION(BlueprintPure, Category="Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();

	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	int32 NumMinions = 5;

	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	float MinSpawnDistance = 100.f;

	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	float MaxSpawnDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, Category="Summoning")
	float SpawnSpread = 90.f;
};
