// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EntombedGameModeBase.generated.h"

class UProfessionInfo;

/**
 * 
 */
UCLASS()
class ENTOMBED_API AEntombedGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Profession Defaults")
	TObjectPtr<UProfessionInfo> ProfessionInformation;
};
