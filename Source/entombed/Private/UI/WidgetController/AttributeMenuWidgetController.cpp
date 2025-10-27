// Copyright Yooshley


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "EntombedGameplayTags.h"
#include "AbilitySystem/EntombedAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfoDataAsset.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	check(AttributeInformation);
	for (auto& Tag : AttributeInformation.Get()->AttributeInformation)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Tag.Attribute).AddLambda(
			[this, Tag](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Tag.AttributeTag);
			});
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	
	check(AttributeInformation);
	for (FEntombedAttributeInfo& Tag : AttributeInformation.Get()->AttributeInformation)
	{
		BroadcastAttributeInfo(Tag.AttributeTag);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag) const
{
	FEntombedAttributeInfo Info = AttributeInformation->FindAttributeInfoByTag(Tag);
	Info.AttributeValue = Info.Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
