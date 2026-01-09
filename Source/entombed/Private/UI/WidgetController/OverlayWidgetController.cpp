// Copyright Yooshley


#include "UI/WidgetController/OverlayWidgetController.h"

#include "EntombedGameplayTags.h"
#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "AbilitySystem/EntombedAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "Player/EntombedPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	const UEntombedAttributeSet* EntombedAttributeSet = GetEntombedAttributeSet();
	
	OnLifeChanged.Broadcast(EntombedAttributeSet->GetLife());
	OnTotalLifeChanged.Broadcast(EntombedAttributeSet->GetTotalLife());
	OnFormChanged.Broadcast(EntombedAttributeSet->GetForm());
	OnTotalFormChanged.Broadcast(EntombedAttributeSet->GetTotalForm());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	AEntombedPlayerState* EntombedPlayerState = GetEntombedPlayerState();
	
	EntombedPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	EntombedPlayerState->OnLevelChangedDelegate.AddLambda([this](int32 NewLevel, bool bLevelUp)
	{
		OnLevelChangedDelegate.Broadcast(NewLevel, bLevelUp);
	});
	
	const UEntombedAttributeSet* EntombedAttributeSet = GetEntombedAttributeSet();
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EntombedAttributeSet->GetLifeAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnLifeChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EntombedAttributeSet->GetTotalLifeAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnTotalLifeChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EntombedAttributeSet->GetFormAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnFormChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EntombedAttributeSet->GetTotalFormAttribute()).AddLambda(
[this](const FOnAttributeChangeData& Data)
	{
		OnTotalFormChanged.Broadcast(Data.NewValue);
	});

	if (GetEntombedAbilitySystemComponent())
	{
		EntombedASC->AbilityEquippedDelegate.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		if (EntombedASC->bGrantedDefaultAbilities)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			EntombedASC->GrantedAbilitiesDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}

		EntombedASC->EffectAssetTags.AddLambda(
	[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message.Potion")); //TODO: gameplay tag magic string
				if (Tag.MatchesTag(MessageTag))
				{
					FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		});
	}
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const ULevelInfo* LevelInfo = GetEntombedPlayerState()->LevelInfo;
	checkf(LevelInfo, TEXT("LevelInfo not found in PlayerState"));

	const int32 Level = LevelInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelInfo->LevelInformation.Num() - 1;

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 CurrentXPRequired = LevelInfo->LevelInformation[Level].XPRequirement;
		const int32 PreviousXPRequired = LevelInfo->LevelInformation[Level-1].XPRequirement;

		const int32 DeltaXPRequired = CurrentXPRequired - PreviousXPRequired;
		const int32 XPRequired = NewXP - PreviousXPRequired;

		const float XPPercentage = static_cast<float>(XPRequired) / static_cast<float>(DeltaXPRequired);

		OnXPChangedDelegate.Broadcast(XPPercentage);
	}

}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,
	const FGameplayTag& SlotTag, const FGameplayTag& PrevSlotTag) const
{
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
}
