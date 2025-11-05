// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/ProfessionInfo.h"
#include "Character/EntombedBaseCharacter.h"
#include "Interaction/TargetInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "EntombedEnemyCharacter.generated.h"

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

	/** TargetInterface start */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** TargetInterface end */

	/** CombatInterface start */
	virtual int32 GetCharacterLevel() override;
	virtual void Death() override;
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

protected:
	virtual void BeginPlay() override;
	virtual void InitializeAbilityActorInfo() override;
	virtual  void InitializeDefaultAttributes() const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ProfessionDefaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ProfessionDefaults")
	EProfession Profession = EProfession::Crusader;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> LifeBar;
};
