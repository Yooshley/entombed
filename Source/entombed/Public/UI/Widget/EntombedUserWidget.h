// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EntombedUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
