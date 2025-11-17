// Copyright Yooshley


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "AbilitySystem/EntombedAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/EntombedPlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	AEntombedPlayerState* EntombedPlayerState = CastChecked<AEntombedPlayerState>(PlayerState);
	EntombedPlayerState->OnAttributePointsChangedDelegate.AddLambda([this](int32 NewValue)
	{
		AttributePointsChangedDelegate.Broadcast(NewValue);
	});

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

	AEntombedPlayerState* EntombedPlayerState = CastChecked<AEntombedPlayerState>(PlayerState);
	AttributePointsChangedDelegate.Broadcast(EntombedPlayerState->GetAttributePoints());
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UEntombedAbilitySystemComponent* EntombedASC = CastChecked<UEntombedAbilitySystemComponent>(AbilitySystemComponent);
	EntombedASC->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag) const
{
	FEntombedAttributeInfo Info = AttributeInformation->FindAttributeInfoByTag(Tag);
	Info.AttributeValue = Info.Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
