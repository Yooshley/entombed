// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_MainMenu.generated.h"

class UMVVM_Slot;
/**
 * 
 */
UCLASS()
class ENTOMBED_API UMVVM_MainMenu : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	void InitializeSlots();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_Slot>	SlotViewModelClass;
	
	UFUNCTION(BlueprintPure)
	UMVVM_Slot* GetSlotViewModelByIndex(int32 Index) const;
	
	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void SaveSlotButtonPressed(int32 SlotIndex, const FString& EnteredName);
	
	UFUNCTION(BlueprintCallable)
	void LoadSlotButtonPressed(int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void DeleteSlotButtonPressed(int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void LoadSlotData();
	
private:
	UPROPERTY()
	TMap<int32, UMVVM_Slot*> SlotsMap;
	
	UPROPERTY()
	TObjectPtr<UMVVM_Slot> Slot_0;
	
	UPROPERTY()
	TObjectPtr<UMVVM_Slot> Slot_1;
	
	UPROPERTY()
	TObjectPtr<UMVVM_Slot> Slot_2;
};
