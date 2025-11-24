// Copyright Yooshley


#include "UI/WidgetController/AbilityMenuWidgetController.h"

#include "EntombedGameplayTags.h"
#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/EntombedPlayerState.h"

void UAbilityMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	AbilityPointsChanged.Broadcast(	GetEntombedPlayerState()->GetAbilityPoints());
}

void UAbilityMenuWidgetController::BindCallbacksToDependencies()
{
	GetEntombedAbilitySystemComponent()->AbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
	{
		if (SelectedAbility.AbilityTag.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.StatusTag = StatusTag;
			bool bShouldEnableSpend = false;
			bool bShouldEnableEquip = false;
			ShouldEnableButtons(StatusTag, CurrentAbilityPoints, bShouldEnableSpend, bShouldEnableEquip);
			FString LevelDescription;
			FString NextLevelDescription;
			GetEntombedAbilitySystemComponent()->GetDescriptionByAbilityTag(AbilityTag, LevelDescription, NextLevelDescription);
			AbilitySelectedDelegate.Broadcast(bShouldEnableSpend, bShouldEnableEquip, LevelDescription, NextLevelDescription);
		}
		
		if (AbilityInfo)
		{
			FEntombedAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	GetEntombedAbilitySystemComponent()->AbilityEquippedDelegate.AddUObject(this, &UAbilityMenuWidgetController::OnAbilityEquipped);

	GetEntombedPlayerState()->OnAbilityPointsChangedDelegate.AddLambda([this](int32 AbilityPoints)
	{
		AbilityPointsChanged.Broadcast(AbilityPoints);
		CurrentAbilityPoints = AbilityPoints;
		
		bool bShouldEnableSpend = false;
		bool bShouldEnableEquip = false;
		ShouldEnableButtons(SelectedAbility.StatusTag, CurrentAbilityPoints, bShouldEnableSpend, bShouldEnableEquip);
		FString LevelDescription;
		FString NextLevelDescription;
		GetEntombedAbilitySystemComponent()->GetDescriptionByAbilityTag(SelectedAbility.AbilityTag, LevelDescription, NextLevelDescription);
		AbilitySelectedDelegate.Broadcast(bShouldEnableSpend, bShouldEnableEquip, LevelDescription, NextLevelDescription);
	});
}

void UAbilityMenuWidgetController::AbilitySelected(const FGameplayTag& AbilityTag)
{
	if (bAwaitingSelection)
	{
		const FGameplayTag SelectedTypeTag = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.AbilityTag).TypeTag;
		EndAwaitSelectionDelegate.Broadcast(SelectedTypeTag);
		bAwaitingSelection = false;
	}
	
	const FEntombedGameplayTags GameplayTags = FEntombedGameplayTags::Get();
	const int32 AbilityPoints = GetEntombedPlayerState()->GetAbilityPoints();
	const FGameplayAbilitySpec* Spec = GetEntombedAbilitySystemComponent()->GetSpecFromAbilityTag(AbilityTag);
	FGameplayTag StatusTag;
	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(GameplayTags.Ability_None) || Spec == nullptr)
	{
		StatusTag = GameplayTags.Ability_Status_Locked;
	}
	else
	{
		StatusTag = GetEntombedAbilitySystemComponent()->GetStatusTagFromSpec(*Spec);
	}

	SelectedAbility.AbilityTag = AbilityTag;
	SelectedAbility.StatusTag = StatusTag;

	bool bShouldEnableSpend = false;
	bool bShouldEnableEquip = false;
	ShouldEnableButtons(StatusTag, AbilityPoints, bShouldEnableSpend, bShouldEnableEquip);
	FString LevelDescription; 
	FString NextLevelDescription;
	GetEntombedAbilitySystemComponent()->GetDescriptionByAbilityTag(AbilityTag, LevelDescription, NextLevelDescription);
	AbilitySelectedDelegate.Broadcast(bShouldEnableSpend, bShouldEnableEquip, LevelDescription, NextLevelDescription);
}

void UAbilityMenuWidgetController::AbilityDeselected()
{
	if (bAwaitingSelection)
	{
		const FGameplayTag SelectedTypeTag = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.AbilityTag).TypeTag;
		EndAwaitSelectionDelegate.Broadcast(SelectedTypeTag);
		bAwaitingSelection = false;
	}
	
	SelectedAbility.AbilityTag = FEntombedGameplayTags::Get().Ability_None;
	SelectedAbility.StatusTag = FEntombedGameplayTags::Get().Ability_Status_Locked;
	AbilitySelectedDelegate.Broadcast(false, false, FString(), FString());
}

void UAbilityMenuWidgetController::SpendButtonPressed()
{
	if(GetEntombedAbilitySystemComponent())
	{
		GetEntombedAbilitySystemComponent()->ServerSpendAbilityPoint(SelectedAbility.AbilityTag);
	}
}

void UAbilityMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag TypeTag = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.AbilityTag).TypeTag;
	BeginAwaitSelectionDelegate.Broadcast(TypeTag);
	bAwaitingSelection = true;

	const FGameplayTag SelectedStatusTag = GetEntombedAbilitySystemComponent()->GetStatusTagFromAbilityTag(SelectedAbility.AbilityTag);
	if(SelectedStatusTag.MatchesTag(FEntombedGameplayTags::Get().Ability_Status_Equipped))
	{
		SelectedSlotTag = GetEntombedAbilitySystemComponent()->GetInputTagFromAbilityTag(SelectedAbility.AbilityTag);
	}
}

void UAbilityMenuWidgetController::AbilityRowButtonPressed(const FGameplayTag& SlotTag, const FGameplayTag& TypeTag)
{
	if(!bAwaitingSelection) return;
	const FGameplayTag& SelectedTypeTag = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.AbilityTag).TypeTag;
	if (!SelectedTypeTag.MatchesTagExact(TypeTag)) return;

	GetEntombedAbilitySystemComponent()->ServerEquipAbility(SelectedAbility.AbilityTag, SlotTag);
}

void UAbilityMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,
	const FGameplayTag& SlotTag, const FGameplayTag& PrevSlotTag)
{
	bAwaitingSelection = false;
	
	const FEntombedGameplayTags& GameplayTags = FEntombedGameplayTags::Get();
	
	FEntombedAbilityInfo PrevAbilityInfo;
	PrevAbilityInfo.StatusTag = GameplayTags.Ability_Status_Unlocked;
	PrevAbilityInfo.InputTag = PrevSlotTag;
	PrevAbilityInfo.AbilityTag = GameplayTags.Ability_None;

	AbilityInfoDelegate.Broadcast(PrevAbilityInfo); //clear out prev slot

	FEntombedAbilityInfo NewAbilityInfo = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
	NewAbilityInfo.StatusTag = StatusTag;
	NewAbilityInfo.InputTag = SlotTag;

	AbilityInfoDelegate.Broadcast(NewAbilityInfo); //fill in new slot

	EndAwaitSelectionDelegate.Broadcast(AbilityInfo->FindAbilityInfoByTag(AbilityTag).TypeTag);
	AbilityReassignedDelegate.Broadcast(AbilityTag);
	AbilityDeselected();
}

void UAbilityMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 AbilityPoints,
                                                       bool& bShouldEnableSpendButton, bool& bShouldEnableEquipButton)
{
	const FEntombedGameplayTags GameplayTags = FEntombedGameplayTags::Get();
	bShouldEnableSpendButton = false;
	bShouldEnableEquipButton = false;
	
	if (AbilityStatus.MatchesTagExact(GameplayTags.Ability_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (AbilityPoints > 0)
		{
			bShouldEnableSpendButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Ability_Status_Eligible))
	{
		bShouldEnableEquipButton = false;
		if (AbilityPoints > 0)
		{
			bShouldEnableSpendButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Ability_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (AbilityPoints > 0)
		{
			bShouldEnableSpendButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Ability_Status_Locked))
	{
		bShouldEnableEquipButton = false;
		if (AbilityPoints > 0)
		{
			bShouldEnableSpendButton = false;
		}
	}
}
