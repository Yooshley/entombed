// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EntombedWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParameters
{
	GENERATED_BODY()

	FWidgetControllerParameters() {}
	FWidgetControllerParameters(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};
/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedWidgetController : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	void SetWidgetControllerParameters(FWidgetControllerParameters Parameters);
	
	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	virtual void BroadcastInitialValues();
	
	virtual void BindCallbacksToDependencies();
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
