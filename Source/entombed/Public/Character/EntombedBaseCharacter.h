// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/ArchetypeInfo.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "EntombedBaseCharacter.generated.h"

class UWidgetComponent;
enum class EEntombedArchetype : uint8;
class UNiagaraSystem;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS(Abstract)
class ENTOMBED_API AEntombedBaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AEntombedBaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
	FVector StrafeTarget = FVector::ZeroVector;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	/* CombatInterface begin */
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Death() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag ) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatarActor_Implementation() override;
	virtual UNiagaraSystem* GetImpactEffect_Implementation() override;
	virtual TArray<FTaggedMontage> GetTaggedMontages_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual void SetTarget_Implementation(AActor* InTarget) override;
	virtual AActor* GetTarget_Implementation() const override;
	virtual FRotator GetTargetDirection_Implementation() const override;
	virtual void SetTargetDirection_Implementation(FVector TargetLocation) override;
	virtual void SetOrientationMode_Implementation(const bool bStrafe) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void SetMinionCount_Implementation(int32 Count) override;
	/* CombatInterface end */

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnLifeChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnTotalLifeChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	TObjectPtr<AActor> TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	FRotator TargetDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	int32 MinionCount;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> TaggedMontages;
	
protected:
	virtual void BeginPlay() override;
	virtual void InitializeAbilityActorInfo();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> LifeBar;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY()
	TArray<TObjectPtr<USkeletalMeshComponent>> EquipmentSlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment")
	TObjectPtr<USkeletalMeshComponent> HeadEquipment;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment")
	TObjectPtr<USkeletalMeshComponent> TorsoEquipment;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment")
	TObjectPtr<USkeletalMeshComponent> ArmLeftEquipment;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment")
	TObjectPtr<USkeletalMeshComponent> ArmRightEquipment;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment")
	TObjectPtr<USkeletalMeshComponent> LegLeftEquipment;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment")
	TObjectPtr<USkeletalMeshComponent> LegRightEquipment;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attachment")
	TObjectPtr<USkeletalMeshComponent> HeadAttachment;

	UPROPERTY(EditAnywhere, Category="Equipment")
	TObjectPtr<USkeletalMeshComponent> MainHandEquipment;

	UPROPERTY(EditAnywhere, Category="Equipment")
	TObjectPtr<USkeletalMeshComponent> OffHandEquipment;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultCoreAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultDerivedAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultResourceAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes")
	float BaseRunSpeed = 750.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ArchetypeDefaults")
	EEntombedArchetype Archetype = EEntombedArchetype::Templar;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;

	bool bDead = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	USoundBase* DeathSound;

	/* Dissolve Effect */
	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(const TArray<UMaterialInstanceDynamic*>& DynamicMaterialInstances);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	UNiagaraSystem* ImpactEffect;

private:
	UFUNCTION()
	USkeletalMeshComponent* CreateEquipmentSlot(const FName& Name);
	
	UFUNCTION()
	void SetupDefaultEquipment();

	UPROPERTY(EditAnywhere, Category="Abilities")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
