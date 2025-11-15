// Copyright Yooshley


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "AbilitySystem/EntombedAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "Player/EntombedPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	const UEntombedAttributeSet* EntombedAttributeSet = CastChecked<UEntombedAttributeSet>(AttributeSet);
	OnLifeChanged.Broadcast(EntombedAttributeSet->GetLife());
	OnMaxLifeChanged.Broadcast(EntombedAttributeSet->GetTotalLife());
	OnManaChanged.Broadcast(EntombedAttributeSet->GetMind());
	OnMaxManaChanged.Broadcast(EntombedAttributeSet->GetTotalMind());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	AEntombedPlayerState* EntombedPlayerState = CastChecked<AEntombedPlayerState>(PlayerState);
	EntombedPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	
	const UEntombedAttributeSet* EntombedAttributeSet = CastChecked<UEntombedAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EntombedAttributeSet->GetLifeAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnLifeChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EntombedAttributeSet->GetTotalLifeAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnMaxLifeChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EntombedAttributeSet->GetMindAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnManaChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EntombedAttributeSet->GetTotalMindAttribute()).AddLambda(
[this](const FOnAttributeChangeData& Data)
	{
		OnMaxManaChanged.Broadcast(Data.NewValue);
	});

	if (UEntombedAbilitySystemComponent* EntombedASC = Cast<UEntombedAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (EntombedASC->bGrantedDefaultAbilities)
		{
			OnInitializeDefaultAbilities(EntombedASC);
		}
		else
		{
			EntombedASC->GrantedAbilitiesDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeDefaultAbilities);
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

void UOverlayWidgetController::OnInitializeDefaultAbilities(UEntombedAbilitySystemComponent* EntombedASC)
{
	if (!EntombedASC->bGrantedDefaultAbilities) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, EntombedASC](const FGameplayAbilitySpec& AbilitySpec)
	{
		FEntombedAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(EntombedASC->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = EntombedASC->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	EntombedASC->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const AEntombedPlayerState* EntombedPlayerState = CastChecked<AEntombedPlayerState>(PlayerState);
	const ULevelInfo* LevelInfo = EntombedPlayerState->LevelInfo;
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
