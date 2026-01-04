// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "Character/EntombedBaseCharacter.h"
#include "Engine/TargetPoint.h"
#include "EntombedSpawnPoint.generated.h"

enum class EEntombedArchetype : uint8;
class AEntombedEnemyCharacter;
/**
 * 
 */
UCLASS()
class ENTOMBED_API AEntombedSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Spawn();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn Class")
	TSubclassOf<AEntombedEnemyCharacter> SpawnClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn Class")
	int32 Level = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn Class")
	EEntombedArchetype Archetype = EEntombedArchetype::Minion_Melee;
};
