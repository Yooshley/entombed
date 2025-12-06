// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAbilitySystemComponent;
class UNiagaraSystem;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilitySystemReady, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamageSignature, float /* DamageAmount */)

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;
};

UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ENTOMBED_API ICombatInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	int32 GetCharacterLevel();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& SocketTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetTarget(AActor* InTarget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetTarget() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetOrientationMode(const bool bStrafe);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FRotator GetTargetDirection() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetTargetDirection(FVector TargetLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCasting(const bool bCast);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetCasting() const;

	virtual void Death(const FVector& DeathImpulse) = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	virtual FOnDeathSignature& GetOnDeathDelegate() = 0;

	virtual FOnDamageSignature& GetOnDamageDelegate() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatarActor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetTaggedMontages();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetImpactEffect();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMinionCount();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMinionCount(int32 Count);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EEntombedArchetype GetArchetype() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetMainHandEquipment();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetOffHandEquipment();

	virtual FOnAbilitySystemReady& GetOnAbilitySystemReadyDelegate() = 0;
};
