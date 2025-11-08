// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/ArchetypeInfo.h"
#include "Character/EntombedBaseCharacter.h"
#include "Interaction/TargetInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "EntombedEnemyCharacter.generated.h"

class UBehaviorTree;
class AEntombedAIController;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class ENTOMBED_API AEntombedEnemyCharacter : public AEntombedBaseCharacter, public ITargetInterface
{
	GENERATED_BODY()

public:
	AEntombedEnemyCharacter();
	virtual void PossessedBy(AController* NewController) override;

	/** TargetInterface start */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** TargetInterface end */

	/** CombatInterface start */
	virtual int32 GetCharacterLevel() override;
	virtual void Death() override;
	virtual void SetTarget_Implementation(AActor* InTarget) override;
	virtual AActor* GetTarget_Implementation() const override;
	/** CombatInterface end */

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnLifeChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnTotalLifeChanged;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float LifeSpanPostDeath = 5.f;

	UPROPERTY(BlueprintReadWrite, Category="Combat")
	TObjectPtr<AActor> TargetActor;

protected:
	virtual void BeginPlay() override;
	virtual void InitializeAbilityActorInfo() override;
	virtual  void InitializeDefaultAttributes() const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ArchetypeDefaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> LifeBar;

	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AEntombedAIController> EntombedAIController;
};
