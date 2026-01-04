// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/ArchetypeInfo.h"
#include "GameFramework/Character.h"
#include "entombed/Public/Interface/CombatInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "EntombedBaseCharacter.generated.h"

class UPassiveNiagaraComponent;
class UDebuffNiagaraComponent;
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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	bool bIsCasting = false;
	FVector StrafeTarget = FVector::ZeroVector;

	UPROPERTY(ReplicatedUsing=OnRep_Shocked, BlueprintReadOnly)
	bool bIsShocked = false;

	UPROPERTY(ReplicatedUsing=OnRep_Burned, BlueprintReadOnly)
	bool bIsBurned = false;

	UFUNCTION()
	virtual void OnRep_Shocked();

	UFUNCTION()
	virtual void OnRep_Burned();

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	/* CombatInterface begin */
	virtual int32 GetCharacterLevel_Implementation() override;
	virtual void SetCharacterLevel_Implementation(int32 InLevel) override;
	virtual FOnAbilitySystemReady& GetOnAbilitySystemReadyDelegate() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual void Death(const FVector& DeathImpulse) override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag ) override;
	virtual bool IsDead_Implementation() const override;
	virtual void SetCasting_Implementation(const bool bCast) override;
	virtual bool GetCasting_Implementation() const override; 
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
	virtual EEntombedArchetype GetArchetype_Implementation() const override;
	virtual void SetArchetype_Implementation(EEntombedArchetype InArchetype) override;
	virtual USkeletalMeshComponent* GetMainHandEquipment_Implementation() override;
	virtual USkeletalMeshComponent* GetOffHandEquipment_Implementation() override;
	virtual FOnDamageSignature& GetOnDamageDelegate() override;
	/* CombatInterface end */
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ArchetypeDefaults")
	int32 Level = 1;

	FOnAbilitySystemReady OnAbilitySystemReady;
	FOnDeathSignature OnDeath;
	FOnDamageSignature OnDamage;

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

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float InLevel) const;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<UDebuffNiagaraComponent> ShockDebuffComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<UPassiveNiagaraComponent> PassiveDefenseComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<UPassiveNiagaraComponent> PassiveSiphonComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;
	
private:
	UFUNCTION()
	USkeletalMeshComponent* CreateEquipmentSlot(const FName& Name);
	
	UFUNCTION()
	void SetupDefaultEquipment();

	UPROPERTY(EditAnywhere, Category="Abilities")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
