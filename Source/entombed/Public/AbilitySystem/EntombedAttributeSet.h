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

	/*
	 * RESOURCE ATTRIBUTES
	*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Life, Category="Attributes|Resource")
	FGameplayAttributeData Life;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Life);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_TotalLife, Category="Attributes|Resource")
	FGameplayAttributeData TotalLife;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, TotalLife);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Form, Category="Attributes|Resource")
	FGameplayAttributeData Form;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Form);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_TotalForm, Category="Attributes|Resource")
	FGameplayAttributeData TotalForm;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, TotalForm);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mind, Category="Attributes|Resource")
	FGameplayAttributeData Mind;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Mind);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_TotalMind, Category="Attributes|Resource")
	FGameplayAttributeData TotalMind;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, TotalMind);

	/*
	 * CORE ATTRIBUTES
	*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category="Attributes|Core")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Vigor); // Templar(I), Barbarian(II); TotalLife

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Instinct, Category="Attributes|Core")
	FGameplayAttributeData Instinct;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Instinct); // Barbarian(I), Saboteur(II); FormRegen, BlockChance

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Technique, Category="Attributes|Core")
	FGameplayAttributeData Technique;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Technique); // Saboteur(I), Mechanist(II); TotalForm, CritChance

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Acumen, Category="Attributes|Core")
	FGameplayAttributeData Acumen;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Acumen); // Sorcerer(I), Ritualist(II); MindRegen

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Logic, Category="Attributes|Core")
	FGameplayAttributeData Logic;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Logic); // Mechanist(I), Sorcerer(II); TotalMind

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Spirit, Category="Attributes|Core")
	FGameplayAttributeData Spirit;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Spirit); //Ritualist(I), Templar(II); LifeRegen

	/*
	 * DERIVED ATTRIBUTES
	*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LifeRegeneration, Category="Attributes|Derived")
	FGameplayAttributeData LifeRegeneration;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, LifeRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FormRegeneration, Category="Attributes|Derived")
	FGameplayAttributeData FormRegeneration;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, FormRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MindRegeneration, Category="Attributes|Derived")
	FGameplayAttributeData MindRegeneration;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, MindRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalChance, Category="Attributes|Derived")
	FGameplayAttributeData CriticalChance;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, CriticalChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalChance, Category="Attributes|Derived")
	FGameplayAttributeData CriticalMultiplier;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, CriticalMultiplier);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EvadeChance, Category="Attributes|Derived")
	FGameplayAttributeData EvadeChance;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, EvadeChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category="Attributes|Derived")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorRating, Category="Attributes|Derived")
	FGameplayAttributeData ArmorRating;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, ArmorRating);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorRating, Category="Attributes|Derived")
	FGameplayAttributeData BurnResistance;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, BurnResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorRating, Category="Attributes|Derived")
	FGameplayAttributeData ShockResistance;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, ShockResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorRating, Category="Attributes|Derived")
	FGameplayAttributeData FreezeResistance;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, FreezeResistance);

	/*
	 * META ATTRIBUTES
	*/
	UPROPERTY(BlueprintReadOnly, Category="Attributes|Meta")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, IncomingDamage);
	
	UFUNCTION()
	void OnRep_Life(const FGameplayAttributeData& OldLife) const;

	UFUNCTION()
	void OnRep_TotalLife(const FGameplayAttributeData& OldTotalLife) const;

	UFUNCTION()
	void OnRep_Form(const FGameplayAttributeData& OldForm) const;

	UFUNCTION()
	void OnRep_TotalForm(const FGameplayAttributeData& OldTotalForm) const;

	UFUNCTION()
	void OnRep_Mind(const FGameplayAttributeData& OldMind) const;

	UFUNCTION()
	void OnRep_TotalMind(const FGameplayAttributeData& OldTotalMind) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void OnRep_Instinct(const FGameplayAttributeData& OldInstinct) const;

	UFUNCTION()
	void OnRep_Technique(const FGameplayAttributeData& OldTechnique) const;
	
	UFUNCTION()
	void OnRep_Acumen(const FGameplayAttributeData& OldAcumen) const;

	UFUNCTION()
	void OnRep_Logic(const FGameplayAttributeData& OldLogic) const;

	UFUNCTION()
	void OnRep_Spirit(const FGameplayAttributeData& OldSpirit) const;

	UFUNCTION()
	void OnRep_LifeRegeneration(const FGameplayAttributeData& OldLifeRegeneration) const;

	UFUNCTION()
	void OnRep_FormRegeneration(const FGameplayAttributeData& OldFormRegeneration) const;

	UFUNCTION()
	void OnRep_MindRegeneration(const FGameplayAttributeData& OldMindRegeneration) const;

	UFUNCTION()
	void OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance) const;

	UFUNCTION()
	void OnRep_CriticalMultiplier(const FGameplayAttributeData& OldCriticalMultiplier) const;

	UFUNCTION()
	void OnRep_EvadeChance(const FGameplayAttributeData& OldEvadeChance) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

	UFUNCTION()
	void OnRep_ArmorRating(const FGameplayAttributeData& OldArmorRating) const;

	UFUNCTION()
	void OnRep_BurnResistance(const FGameplayAttributeData& OldBurnResistance) const;

	UFUNCTION()
	void OnRep_ShockResistance(const FGameplayAttributeData& OldShockResistance) const;
	
	UFUNCTION()
	void OnRep_FreezeResistance(const FGameplayAttributeData& OldFreezeResistance) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Properties) const;

	const void ShowFloatingText(const FEffectProperties& Properties, float Damage, bool bBlockedHit, bool bCriticalHit);
};
