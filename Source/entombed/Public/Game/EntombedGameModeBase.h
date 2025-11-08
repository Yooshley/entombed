// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EntombedGameModeBase.generated.h"

class UArchetypeInfo;

/**
 * 
 */
UCLASS()
class ENTOMBED_API AEntombedGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Archetype Defaults")
	TObjectPtr<UArchetypeInfo> ArchetypeInformation;
};
