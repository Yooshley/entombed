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

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()
	
	FEffectProperties() {}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceAbilitySystemComponent = nullptr;
	
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
	 * CORE ATTRIBUTES
	*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Life, Category="Attributes|Core")
	FGameplayAttributeData Life;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Life);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxLife, Category="Attributes|Core")
	FGameplayAttributeData MaxLife;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, MaxLife);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Form, Category="Attributes|Core")
	FGameplayAttributeData Form;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Form);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxForm, Category="Attributes|Core")
	FGameplayAttributeData MaxForm;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, MaxForm);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mind, Category="Attributes|Core")
	FGameplayAttributeData Mind;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Mind);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMind, Category="Attributes|Core")
	FGameplayAttributeData MaxMind;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, MaxMind);

	/*
	 * PRIMARY ATTRIBUTES
	*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category="Attributes|Primary")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Vigor); // Templar(I), Barbarian(II); MaxLife

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Instinct, Category="Attributes|Primary")
	FGameplayAttributeData Instinct;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Instinct); // Barbarian(I), Saboteur(II); FormRegen, EvadeChance

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Technique, Category="Attributes|Primary")
	FGameplayAttributeData Technique;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Technique); // Saboteur(I), Mechanist(II); MaxForm, CritChance

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Acumen, Category="Attributes|Primary")
	FGameplayAttributeData Acumen;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Acumen); // Sorcerer(I), Ritualist(II); MindRegen

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Logic, Category="Attributes|Primary")
	FGameplayAttributeData Logic;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Logic); // Mechanist(I), Sorcerer(II); MaxMind

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Spirit, Category="Attributes|Primary")
	FGameplayAttributeData Spirit;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Spirit); //Ritualist(I), Templar(II); LifeRegen

	/*
	 * SECONDARY ATTRIBUTES
	*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LifeRegeneration, Category="Attributes|Secondary")
	FGameplayAttributeData LifeRegeneration;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, LifeRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FormRegeneration, Category="Attributes|Secondary")
	FGameplayAttributeData FormRegeneration;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, FormRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MindRegeneration, Category="Attributes|Secondary")
	FGameplayAttributeData MindRegeneration;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, MindRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalChance, Category="Attributes|Secondary")
	FGameplayAttributeData CriticalChance;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, CriticalChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EvasionChance, Category="Attributes|Secondary")
	FGameplayAttributeData EvasionChance;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, EvasionChance);
	
	UFUNCTION()
	void OnRep_Life(const FGameplayAttributeData& OldLife) const;

	UFUNCTION()
	void OnRep_MaxLife(const FGameplayAttributeData& OldMaxLife) const;

	UFUNCTION()
	void OnRep_Form(const FGameplayAttributeData& OldGrit) const;

	UFUNCTION()
	void OnRep_MaxForm(const FGameplayAttributeData& OldMaxGrit) const;

	UFUNCTION()
	void OnRep_Mind(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMind(const FGameplayAttributeData& OldMaxMana) const;

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
	void OnRep_EvasionChance(const FGameplayAttributeData& OldEvasionChance) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Properties) const;
};
