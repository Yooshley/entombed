// Copyright Yooshley


#include "UI/ViewModel/MVVM_MainMenu.h"

#include "Game/EntombedGameInstance.h"
#include "Game/EntombedGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_Slot.h"

void UMVVM_MainMenu::InitializeSlots()
{
	Slot_0 = NewObject<UMVVM_Slot>(this, SlotViewModelClass);
	Slot_0->SlotName = FString("Slot 0");
	SlotsMap.Add(0, Slot_0);
	
	Slot_1 = NewObject<UMVVM_Slot>(this, SlotViewModelClass);
	Slot_1->SlotName = FString("Slot 1");
	SlotsMap.Add(1, Slot_1);
	
	Slot_2 = NewObject<UMVVM_Slot>(this, SlotViewModelClass);
	Slot_2->SlotName = FString("Slot 2");
	SlotsMap.Add(2, Slot_2);
}

UMVVM_Slot* UMVVM_MainMenu::GetSlotViewModelByIndex(int32 Index) const
{
	return SlotsMap.FindChecked(Index);
}

void UMVVM_MainMenu::NewSlotButtonPressed(int32 SlotIndex)
{
	SlotsMap[SlotIndex]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_MainMenu::SaveSlotButtonPressed(int32 SlotIndex, const FString& EnteredName)
{
	AEntombedGameModeBase* EntombedGameMode = Cast<AEntombedGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(EntombedGameMode)) return;
	
	SlotsMap[SlotIndex]->SetSavedName(EnteredName);
	SlotsMap[SlotIndex]->SetMapName(EntombedGameMode->DefaultMapName);
	SlotsMap[SlotIndex]->SetLevel(1);
	SlotsMap[SlotIndex]->SlotStatus = Taken;
	SlotsMap[SlotIndex]->PlayerStartTag = EntombedGameMode->DefaultPlayerStartTag;
	SlotsMap[SlotIndex]->MapAssetName = EntombedGameMode->DefaultMap.ToSoftObjectPath().GetAssetName();
	
	EntombedGameMode->SaveSlot(SlotsMap[SlotIndex], SlotIndex);
	SlotsMap[SlotIndex]->InitializeSlot();
	
	UEntombedGameInstance* EntombedGameInstance = Cast<UEntombedGameInstance>(EntombedGameMode->GetGameInstance());
	EntombedGameInstance->SlotName = EnteredName;
	EntombedGameInstance->SlotIndex = SlotIndex;
	EntombedGameInstance->CheckpointTag = EntombedGameMode->DefaultPlayerStartTag;
}

void UMVVM_MainMenu::LoadSlotButtonPressed(int32 SlotIndex)
{
	AEntombedGameModeBase* EntombedGameMode = Cast<AEntombedGameModeBase>(UGameplayStatics::GetGameMode(this));
	UEntombedGameInstance* EntombedGameInstance = Cast<UEntombedGameInstance>(EntombedGameMode->GetGameInstance());
	EntombedGameInstance->CheckpointTag = SlotsMap[SlotIndex]->PlayerStartTag;
	EntombedGameInstance->SlotName = SlotsMap[SlotIndex]->SlotName;
	EntombedGameInstance->SlotIndex = SlotsMap[SlotIndex]->SlotIndex;
	
	EntombedGameMode->TravelToMap(SlotsMap[SlotIndex]);
}

void UMVVM_MainMenu::DeleteSlotButtonPressed(int32 SlotIndex)
{
	UMVVM_Slot* Slot = SlotsMap[SlotIndex];
	if (IsValid(Slot))
	{
		AEntombedGameModeBase::DeleteSlot(Slot, SlotIndex);
		Slot->SlotStatus = Vacant;
		Slot->InitializeSlot();
	}
}

void UMVVM_MainMenu::LoadSlotData()
{
	AEntombedGameModeBase* EntombedGameMode = Cast<AEntombedGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(EntombedGameMode)) return;
	for (const TTuple<int32, UMVVM_Slot*> Slot : SlotsMap)
	{
		UEntombedSaveGame* SaveObject = EntombedGameMode->GetSlotData(Slot.Value->SlotName, Slot.Key);
		
		const FString SavedName = SaveObject->SavedName;
		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;
		
		Slot.Value->SetSavedName(SavedName);
		Slot.Value->SetMapName(SaveObject->MapName);
		Slot.Value->SetLevel(SaveObject->PlayerLevel);
		Slot.Value->PlayerStartTag = SaveObject->CheckpointTag;
		Slot.Value->SlotStatus= SaveSlotStatus;
		Slot.Value->InitializeSlot();
	}
}
