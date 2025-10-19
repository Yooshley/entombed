// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/EntombedWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UEntombedUserWidget;
struct FOnAttributeChangeData;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText MessageText = FText();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UEntombedUserWidget> MessageWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* MessageImage = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ENTOMBED_API UOverlayWidgetController : public UEntombedWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BLueprintAssignable, Category="AbilitySystem|Attributes")
	FOnAttributeChangedSignature OnLifeChanged;

	UPROPERTY(BLueprintAssignable, Category="AbilitySystem|Attributes")
	FOnAttributeChangedSignature OnMaxLifeChanged;

	UPROPERTY(BLueprintAssignable, Category="AbilitySystem|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BLueprintAssignable, Category="AbilitySystem|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BLueprintAssignable, Category="AbilitySystem|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag) // tag name must be row name
{
	return  DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
