// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/EntombedSaveGame.h"
#include "MVVM_Slot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);

/**
 * 
 */
UCLASS()
class ENTOMBED_API UMVVM_Slot : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;
	
	void InitializeSlot();
	
	UPROPERTY()
	FString SlotName;
	
	UPROPERTY()
	int32 SlotIndex;
	
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;
	
	void SetSavedName(FString InSavedName);
	
	FString GetSavedName() const { return SavedName; }
	
	void SetMapName(FString InMapName);
	
	FString GetMapName() const { return MapName; }
	
private:
	/** Field Notifies **/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
    FString SavedName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess))
	FString MapName;
};
