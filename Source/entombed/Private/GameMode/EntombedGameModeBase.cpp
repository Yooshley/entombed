// Copyright Yooshley


#include "entombed/Public/Game/EntombedGameModeBase.h"

#include "Game/EntombedGameInstance.h"
#include "Game/EntombedSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_Slot.h"

void AEntombedGameModeBase::DeleteSlot(UMVVM_Slot* Slot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(Slot->SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(Slot->SlotName, SlotIndex);
	}
}

void AEntombedGameModeBase::SaveSlot(UMVVM_Slot* Slot, int32 SlotIndex)
{
	DeleteSlot(Slot, SlotIndex);
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(SaveGameClass);
	UEntombedSaveGame* SaveGame = Cast<UEntombedSaveGame>(SaveGameObject);
	SaveGame->SavedName = Slot->GetSavedName();
	SaveGame->MapName = Slot->GetMapName();
	SaveGame->SaveSlotStatus = Taken;
	
	UGameplayStatics::SaveGameToSlot(SaveGame, Slot->SlotName, SlotIndex);
}

UEntombedSaveGame* AEntombedGameModeBase::GetSlotData(const FString& SlotName, int32 SlotIndex)
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(SaveGameClass);
	}
	UEntombedSaveGame* EntombedSaveGame = Cast<UEntombedSaveGame>(SaveGameObject);
	return EntombedSaveGame;
}

void AEntombedGameModeBase::TravelToMap(UMVVM_Slot* Slot)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

AActor* AEntombedGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UEntombedGameInstance* EntombedGameInstance = Cast<UEntombedGameInstance>(GetGameInstance());
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	if (Actors.Num() > 0)
	{
		AActor* SelectedActor = Actors[0];
		for (AActor* Actor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == EntombedGameInstance->PlayerStartTag)
				{
					SelectedActor = PlayerStart;
					break;
				}
			}
		}
		return SelectedActor;
	}
	return nullptr;
}

void AEntombedGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(DefaultMapName, DefaultMap);
}
