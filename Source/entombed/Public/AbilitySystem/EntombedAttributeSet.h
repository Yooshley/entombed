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

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Life, Category="Attributes|Vitals")
	FGameplayAttributeData Life;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Life);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxLife, Category="Attributes|Vitals")
	FGameplayAttributeData MaxLife;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, MaxLife);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category="Attributes|Vitals")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category="Attributes|Vitals")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UEntombedAttributeSet, MaxMana);
	
	UFUNCTION()
	void OnRep_Life(const FGameplayAttributeData& PreviousLife) const;

	UFUNCTION()
	void OnRep_MaxLife(const FGameplayAttributeData& PreviousMaxLife) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& PreviousMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& PreviousMaxMana) const;
};
