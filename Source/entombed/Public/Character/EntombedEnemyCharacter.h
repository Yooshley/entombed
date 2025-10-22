// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "Character/EntombedBaseCharacter.h"
#include "Interaction/TargetInterface.h"
#include "EntombedEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API AEntombedEnemyCharacter : public AEntombedBaseCharacter, public ITargetInterface
{
	GENERATED_BODY()

public:
	AEntombedEnemyCharacter();

	/** TargetInterface start */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** TargetInterface end */

	/** CombatInterface start */
	virtual int32 GetCharacterLevel() override;
	/** CombatInterface end */

protected:
	virtual void BeginPlay() override;
	virtual void InitializeAbilityActorInfo() override;
	
	UPROPERTY(VisibleAnywhere, Category="CharacterClassDefaults")
	int32 Level = 1;
};
