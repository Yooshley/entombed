// Copyright Yooshley


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "AbilitySystem/EntombedAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	const UEntombedAttributeSet* EntombedAttributeSet = CastChecked<UEntombedAttributeSet>(AttributeSet);
	OnLifeChanged.Broadcast(EntombedAttributeSet->GetLife());
	OnMaxLifeChanged.Broadcast(EntombedAttributeSet->GetMaxLife());
	OnManaChanged.Broadcast(EntombedAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(EntombedAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	const UEntombedAttributeSet* EntombedAttributeSet = CastChecked<UEntombedAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EntombedAttributeSet->GetLifeAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnLifeChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EntombedAttributeSet->GetMaxLifeAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnMaxLifeChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EntombedAttributeSet->GetManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnManaChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EntombedAttributeSet->GetMaxManaAttribute()).AddLambda(
[this](const FOnAttributeChangeData& Data)
	{
		OnMaxManaChanged.Broadcast(Data.NewValue);
	});
	
	Cast<UEntombedAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
	{
		for (const FGameplayTag& Tag : AssetTags)
		{
			FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message.Potion"));
			if (Tag.MatchesTag(MessageTag))
			{
				FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
				MessageWidgetRowDelegate.Broadcast(*Row);
			}
		}
	});
}