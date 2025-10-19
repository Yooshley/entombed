// Copyright Yooshley


#include "UI/WidgetController/EntombedWidgetController.h"

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

void UEntombedWidgetController::BindCallbacksToDependencies()
{
	
}
