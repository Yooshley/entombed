// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "EntombedBaseCharacter.generated.h"

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

protected:
	virtual void BeginPlay() override;

	virtual void InitializeAbilityActorInfo();

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

	UPROPERTY(EditAnywhere, Category="Equipment")
	FName MainHandTipSocketName;

	UPROPERTY(EditAnywhere, Category="Equipment")
	FName OffHandTipSocketName;

	virtual FVector GetMainHandSocketLocation() override;
	virtual FVector GetOffHandSocketLocation() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultCoreAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultDerivedAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultResourceAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;

	void AddDefaultAbilities();

private:
	UFUNCTION()
	USkeletalMeshComponent* CreateEquipmentSlot(const FName& Name);
	
	UFUNCTION()
	void SetupDefaultEquipment();

	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
};
