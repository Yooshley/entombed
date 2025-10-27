// Copyright Yooshley


#include "UI/HUD/EntombedHUD.h"
#include "UI/Widget/EntombedUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AEntombedHUD::GetOverlayWidgetController(
	const FWidgetControllerParameters& WidgetControllerParameters)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParameters(WidgetControllerParameters);
		OverlayWidgetController->BindCallbacksToDependencies();
		
		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AEntombedHUD::GetAttributeMenuWidgetController(
	const FWidgetControllerParameters& WidgetControllerParameters)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParameters(WidgetControllerParameters);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
		
		return AttributeMenuWidgetController;
	}
	return AttributeMenuWidgetController;
}

void AEntombedHUD::InitializeOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Critical Error! Overlay Widget Class uninitialized."));
	checkf(OverlayWidgetControllerClass, TEXT("Critical Error! Overlay Widget Controller Class uninitialized."))
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UEntombedUserWidget>(Widget);

	const FWidgetControllerParameters WidgetControllerParameters(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParameters);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}
