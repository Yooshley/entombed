// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "Character/EntombedBaseCharacter.h"
#include "EntombedPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API AEntombedPlayerCharacter : public AEntombedBaseCharacter
{
	GENERATED_BODY()

public:
	AEntombedPlayerCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	virtual void InitAbilityActorInfo() override;
};
