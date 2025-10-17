// Copyright Yooshley


#include "UI/Widget/EntombedUserWidget.h"

void UEntombedUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
