// Copyright Yooshley


#include "UI/WidgetController/EntombedWidgetController.h"

#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "AbilitySystem/EntombedAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/EntombedPlayerController.h"
#include "Player/EntombedPlayerState.h"

void UEntombedWidgetController::SetWidgetControllerParameters(FWidgetControllerParameters Parameters)
{
	PlayerController = Parameters.PlayerController;
	PlayerState = Parameters.PlayerState;
	AbilitySystemComponent = Parameters.AbilitySystemComponent;
	AttributeSet = Parameters.AttributeSet;
}

void UEntombedWidgetController::BroadcastInitialValues()
{
}

void UEntombedWidgetController::BroadcastAbilityInfo()
{
	if (!GetEntombedAbilitySystemComponent()->bGrantedDefaultAbilities) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FEntombedAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(EntombedASC->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = EntombedASC->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = EntombedASC->GetStatusTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetEntombedAbilitySystemComponent()->ForEachAbility(BroadcastDelegate);
}

void UEntombedWidgetController::BindCallbacksToDependencies()
{
	
}

AEntombedPlayerController* UEntombedWidgetController::GetEntombedPlayerController()
{
	if (EntombedPC == nullptr)
	{
		EntombedPC = Cast<AEntombedPlayerController>(PlayerController);
	}
	return EntombedPC;
}

AEntombedPlayerState* UEntombedWidgetController::GetEntombedPlayerState()
{
	if (EntombedPS == nullptr)
	{
		EntombedPS = Cast<AEntombedPlayerState>(PlayerState);
	}
	return EntombedPS;
}

UEntombedAbilitySystemComponent* UEntombedWidgetController::GetEntombedAbilitySystemComponent()
{
	if (EntombedASC == nullptr)
	{
		EntombedASC = Cast<UEntombedAbilitySystemComponent>(AbilitySystemComponent);
	}
	return EntombedASC;
}

UEntombedAttributeSet* UEntombedWidgetController::GetEntombedAttributeSet()
{
	if (EntombedAS == nullptr)
	{
		EntombedAS = Cast<UEntombedAttributeSet>(AttributeSet);
	}
	return EntombedAS;
}
