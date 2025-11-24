// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "EntombedGameplayTags.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/EntombedWidgetController.h"
#include "AbilityMenuWidgetController.generated.h"

struct FEntombedGameplayTags;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FAbilitySelectedSignature, bool, bSpendButtonEnabled, bool, bEquipButtonEnabled, FString, LevelDescription, FString, NextLevelDescription);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAwaitSelectionSignature, const FGameplayTag&, TypeTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityReassignedSignature, const FGameplayTag&, AbilityTag);

struct FSelectedAbility
{
	FGameplayTag AbilityTag = FGameplayTag();
	FGameplayTag StatusTag = FGameplayTag();
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ENTOMBED_API UAbilityMenuWidgetController : public UEntombedWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature AbilityPointsChanged;

	UPROPERTY(BlueprintAssignable)
	FAbilitySelectedSignature AbilitySelectedDelegate;
	
	UPROPERTY(BlueprintAssignable)
    FAbilityReassignedSignature AbilityReassignedDelegate;

	UPROPERTY(BlueprintAssignable)
	FAwaitSelectionSignature BeginAwaitSelectionDelegate;
	
	UPROPERTY(BlueprintAssignable)
    FAwaitSelectionSignature EndAwaitSelectionDelegate;

	UFUNCTION(BlueprintCallable)
	void AbilitySelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void AbilityDeselected();

	UFUNCTION(BlueprintCallable)
	void SpendButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void AbilityRowButtonPressed(const FGameplayTag& SlotTag, const FGameplayTag& TypeTag);
	
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& PrevSlotTag);

private:
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 AbilityPoints, bool& bShouldEnableSpendButton, bool& bShouldEnableEquipButton);
	FSelectedAbility SelectedAbility = { FEntombedGameplayTags::Get().Ability_None, FEntombedGameplayTags::Get().Ability_Status_Locked };
	FGameplayTag SelectedSlotTag;
	int32 CurrentAbilityPoints = 0;
	bool bAwaitingSelection = false;
};
