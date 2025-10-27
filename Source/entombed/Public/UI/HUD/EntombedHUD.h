// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EntombedHUD.generated.h"

class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParameters;
class UOverlayWidgetController;
class UEntombedUserWidget;
/**
 * 
 */
UCLASS()
class ENTOMBED_API AEntombedHUD : public AHUD
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<UEntombedUserWidget> OverlayWidget;

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParameters& WidgetControllerParameters);

	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParameters& WidgetControllerParameters);

	void InitializeOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UEntombedUserWidget> OverlayWidgetClass;

	UPROPERTY()
	UOverlayWidgetController* OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	UAttributeMenuWidgetController* AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
};
