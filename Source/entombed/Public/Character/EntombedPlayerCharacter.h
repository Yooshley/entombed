// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "Character/EntombedBaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "entombed/Public/Interface/PlayerInterface.h"
#include "EntombedPlayerCharacter.generated.h"

class UCameraComponent;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class ENTOMBED_API AEntombedPlayerCharacter : public AEntombedBaseCharacter, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AEntombedPlayerCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** PlayerInterface start */
	virtual void AddXP_Implementation(int32 InXP) override;
	virtual void AddLevel_Implementation(int32 InLevel) override;
	virtual void AddAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddAbilityPoints_Implementation(int32 InAbilityPoints) override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 GetAttributePointsAward_Implementation(int32 Level) const override;
	virtual int32 GetAbilityPointsAward_Implementation(int32 Level) const override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetAbilityPoints_Implementation() const override;
	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial = nullptr) override;
	virtual void HideMagicCircle_Implementation() override;
	virtual void SavePlayerProgress_Implementation(const FName& CheckpointTag) override;
	/** PlayerInterface end */

	/** CombatInterface start */
	virtual int32 GetCharacterLevel_Implementation() override;
	virtual void Death(const FVector& DeathImpulse) override;
	/** CombatInterface end */
	
	UPROPERTY(EditDefaultsOnly)
	float DeathTime = 5.f;
	
	FTimerHandle DeathTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	FORCEINLINE FRotator GetCameraBoomRotation() const { return CameraBoom->GetComponentRotation(); }
	
protected:
	virtual void BeginPlay() override;
	void LoadPlayerProgress();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	virtual void InitializeAbilityActorInfo() override;
	virtual void OnRep_Shocked() override;
	virtual void OnRep_Burned() override;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastLevelUpVFX() const;
};
