// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EntombedGameModeBase.generated.h"

class UAbilityInfo;
class UArchetypeInfo;

/**
 * 
 */
UCLASS()
class ENTOMBED_API AEntombedGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Archetype")
	TObjectPtr<UArchetypeInfo> ArchetypeInformation;

	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TObjectPtr<UAbilityInfo> AbilityInformation;
};
