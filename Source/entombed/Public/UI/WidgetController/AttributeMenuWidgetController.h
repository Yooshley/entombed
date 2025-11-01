// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/EntombedWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FGameplayTag;
class UAttributeInfo;
struct FEntombedAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FEntombedAttributeInfo&, Info);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ENTOMBED_API UAttributeMenuWidgetController : public UEntombedWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category="AbilitySystem|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInformation;

private:
	void BroadcastAttributeInfo(const FGameplayTag& Tag) const;
};
