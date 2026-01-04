// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "Character/EntombedBaseCharacter.h"
#include "entombed/Public/Interface/TargetInterface.h"
#include "Interface/HostileInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "EntombedEnemyCharacter.generated.h"

class UBehaviorTree;
class AEntombedAIController;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class ENTOMBED_API AEntombedEnemyCharacter : public AEntombedBaseCharacter, public ITargetInterface, public IHostileInterface
{
	GENERATED_BODY()

public:
	AEntombedEnemyCharacter();
	virtual void PossessedBy(AController* NewController) override;

	/** TargetInterface start */
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	/** TargetInterface end */

	/** CombatInterface start */
	virtual void Death(const FVector& DeathImpulse) override;
	/** CombatInterface end */
	
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float LifeSpanPostDeath = 5.f;

protected:
	virtual void BeginPlay() override;
	virtual void InitializeAbilityActorInfo() override;
	virtual  void InitializeDefaultAttributes() const override;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AEntombedAIController> EntombedAIController;
	
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLoot();
};
