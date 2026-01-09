// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EntombedAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DELEGATE_RetVal(FGameplayAttribute, FAttributeSignature);

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()
	
	FEffectProperties() {}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;
	
	UPROPERTY()
	AController* SourceController = nullptr;
	
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetAbilitySystemComponent = nullptr;
	
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	
	UPROPERTY()
	AController* TargetController = nullptr;
	
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UEntombedAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	void HandleIncomingDamage(const FEffectProperties& Properties);
	void HandleIncomingXP(const FEffectProperties& Properties);
	void HandleIncomingDebuff(const FEffectProperties& Properties);

	/*
	 * ATTRIBUTES
	*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Life, Category="Attributes")
	FGameplayAttributeData Life;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Life);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_TotalLife, Category="Attributes")
	FGameplayAttributeData TotalLife;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, TotalLife);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LifeRegenerationRate, Category="Attributes")
	FGameplayAttributeData LifeRegenerationRate;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, LifeRegenerationRate);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LifeRegenerationDelay, Category="Attributes")
	FGameplayAttributeData LifeRegenerationDelay;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, LifeRegenerationDelay);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Form, Category="Attributes")
	FGameplayAttributeData Form;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Form);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_TotalForm, Category="Attributes")
	FGameplayAttributeData TotalForm;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, TotalForm);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FormRegenerationRate, Category="Attributes")
	FGameplayAttributeData FormRegenerationRate;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, FormRegenerationRate);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FormRegenerationDelay, Category="Attributes")
	FGameplayAttributeData FormRegenerationDelay;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, FormRegenerationDelay);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalChance, Category="Attributes")
	FGameplayAttributeData CriticalChance;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, CriticalChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalChance, Category="Attributes")
	FGameplayAttributeData CriticalMultiplier;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, CriticalMultiplier);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BurnResistance, Category="Attributes")
	FGameplayAttributeData BurnResistance;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, BurnResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ShockResistance, Category="Attributes")
	FGameplayAttributeData ShockResistance;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, ShockResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FreezeResistance, Category="Attributes")
	FGameplayAttributeData FreezeResistance;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, FreezeResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ElementalResistance, Category="Attributes")
	FGameplayAttributeData ElementalResistance;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, ElementalResistance);

	/*
	 * META ATTRIBUTES
	*/
	UPROPERTY(BlueprintReadOnly, Category="Attributes|Meta")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category="Attributes|Meta")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, IncomingXP);
	
	UFUNCTION()
	void OnRep_Life(const FGameplayAttributeData& OldLife) const;

	UFUNCTION()
	void OnRep_TotalLife(const FGameplayAttributeData& OldTotalLife) const;
	
	UFUNCTION()
	void OnRep_LifeRegenerationRate(const FGameplayAttributeData& OldLifeRegenerationRate) const;
	
	UFUNCTION()
	void OnRep_LifeRegenerationDelay(const FGameplayAttributeData& OldLifeRegenerationDelay) const;

	UFUNCTION()
	void OnRep_Form(const FGameplayAttributeData& OldForm) const;

	UFUNCTION()
	void OnRep_TotalForm(const FGameplayAttributeData& OldTotalForm) const;

	UFUNCTION()
	void OnRep_FormRegenerationRate(const FGameplayAttributeData& OldFormRegenerationRate) const;
	
	UFUNCTION()
	void OnRep_FormRegenerationDelay(const FGameplayAttributeData& OldFormRegenerationDelay) const;

	UFUNCTION()
	void OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance) const;

	UFUNCTION()
	void OnRep_CriticalMultiplier(const FGameplayAttributeData& OldCriticalMultiplier) const;

	UFUNCTION()
	void OnRep_BurnResistance(const FGameplayAttributeData& OldBurnResistance) const;

	UFUNCTION()
	void OnRep_ShockResistance(const FGameplayAttributeData& OldShockResistance) const;
	
	UFUNCTION()
	void OnRep_FreezeResistance(const FGameplayAttributeData& OldFreezeResistance) const;
	
	UFUNCTION()
	void OnRep_ElementalResistance(const FGameplayAttributeData& OldElementalResistance) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Properties) const;

	const void ShowFloatingText(const FEffectProperties& Properties, float Damage, bool bBlockedHit, bool bCriticalHit);

	void SendXPEvent(const FEffectProperties& Props) const;

	bool bMaxOutLife = false;
	bool bMaxOutForm = false;
	bool bMaxOutMind = false;
};