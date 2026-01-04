// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/EntombedWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UEntombedAbilitySystemComponent;
class UAbilityInfo;
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelChangedSignature, int32, NewLevel, bool, bLevelUp);
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

	UPROPERTY(BLueprintAssignable, Category="AbilitySystem|Level")
	FOnAttributeChangedSignature OnXPChangedDelegate;

	UPROPERTY(BLueprintAssignable, Category="AbilitySystem|Level")
	FOnLevelChangedSignature OnLevelChangedDelegate;

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

	void OnXPChanged(int32 NewXP);

	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& PrevSlotTag) const;
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag) // tag name must be row name
{
	return  DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
