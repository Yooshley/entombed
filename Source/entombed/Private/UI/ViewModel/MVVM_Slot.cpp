// Copyright Yooshley


#include "UI/ViewModel/MVVM_Slot.h"

void UMVVM_Slot::InitializeSlot()
{
	const int32 SlotWidgetIndex = SlotStatus.GetValue();
	SetWidgetSwitcherIndex.Broadcast(SlotWidgetIndex);
}

void UMVVM_Slot::SetSavedName(FString InSavedName)
{
	UE_MVVM_SET_PROPERTY_VALUE(SavedName, InSavedName);
}

void UMVVM_Slot::SetMapName(FString InMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName);
}
