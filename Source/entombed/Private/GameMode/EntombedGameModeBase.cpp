// Copyright Yooshley


#include "entombed/Public/Game/EntombedGameModeBase.h"

#include "EngineUtils.h"
#include "entombed/EntombedLogChannels.h"
#include "Game/EntombedGameInstance.h"
#include "Game/EntombedSaveGame.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Interface/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UI/ViewModel/MVVM_Slot.h"

class FActorIterator;

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
	SaveGame->MapAssetName = Slot->MapAssetName;
	SaveGame->CheckpointTag = Slot->PlayerStartTag;
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

UEntombedSaveGame* AEntombedGameModeBase::GetGameSaveData()
{
	UEntombedGameInstance* EntombedGameInstance = Cast<UEntombedGameInstance>(GetGameInstance());
	
	const FString GameSlotName = EntombedGameInstance->SlotName;
	const int32 GameSlotIndex = EntombedGameInstance->SlotIndex;
	
	return GetSlotData(GameSlotName, GameSlotIndex);
}

void AEntombedGameModeBase::SaveGameData(UEntombedSaveGame* SaveObject)
{
	UEntombedGameInstance* EntombedGameInstance = Cast<UEntombedGameInstance>(GetGameInstance());
	
	const FString GameSlotName = EntombedGameInstance->SlotName;
	const int32 GameSlotIndex = EntombedGameInstance->SlotIndex;
	EntombedGameInstance->CheckpointTag = SaveObject->CheckpointTag;
	
	UGameplayStatics::SaveGameToSlot(SaveObject, GameSlotName, GameSlotIndex);
}

void AEntombedGameModeBase::SaveWorldState(UWorld* World,  const FString& DestinationMapAssetName)
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);
	
	UEntombedGameInstance* EntombedGameInstance = Cast<UEntombedGameInstance>(GetGameInstance());
	check(EntombedGameInstance);
	
	if(UEntombedSaveGame* SaveGame = GetSlotData(EntombedGameInstance->SlotName, EntombedGameInstance->SlotIndex))
	{
		if (DestinationMapAssetName != FString(""))
		{
			SaveGame->MapAssetName = DestinationMapAssetName;
			SaveGame->MapName = GetMapNameFromMapAssetName(DestinationMapAssetName);
		}
		
		if (!SaveGame->HasMap(WorldName))
		{
			FSavedMap NewSavedMap;
			NewSavedMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add(NewSavedMap);
		}
		
		FSavedMap SavedMap = SaveGame->GetSavedMapFromName(WorldName);
		SavedMap.SavedActors.Empty();
		
		for(FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			
			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;
			
			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.ActorTransform = Actor->GetTransform();
			
			FMemoryWriter MemoryWriter(SavedActor.Bytes);
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;
			
			Actor->Serialize(Archive);
			SavedMap.SavedActors.AddUnique(SavedActor);
		}
		
		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapAssetName == WorldName)
			{
				MapToReplace = SavedMap;
			}
		}
		
		UGameplayStatics::SaveGameToSlot(SaveGame, EntombedGameInstance->SlotName, EntombedGameInstance->SlotIndex);
	}
}

void AEntombedGameModeBase::LoadWorldState(UWorld* World)
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);
	
	UEntombedGameInstance* EntombedGameInstance = Cast<UEntombedGameInstance>(GetGameInstance());
	check(EntombedGameInstance);
	
	if (UGameplayStatics::DoesSaveGameExist(EntombedGameInstance->SlotName, EntombedGameInstance->SlotIndex))
	{
		UEntombedSaveGame* SaveGame = Cast<UEntombedSaveGame>(UGameplayStatics::LoadGameFromSlot(EntombedGameInstance->SlotName, EntombedGameInstance->SlotIndex));
		if (SaveGame == nullptr)
		{
			UE_LOG(LogEntombed, Error, TEXT("Failed to load slot"));
			return;
		}
		
		for(FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			
			if (!Actor->Implements<USaveInterface>()) continue;
			
			for (FSavedActor SavedActor : SaveGame->GetSavedMapFromName(WorldName).SavedActors)
			{
				if (SavedActor.ActorName == Actor->GetFName())
				{
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SavedActor.ActorTransform);
					}
					
					FMemoryReader MemoryReader(SavedActor.Bytes);
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive);
					
					ISaveInterface::Execute_LoadActor(Actor);
				}
			}
		}
	}
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
				if (PlayerStart->PlayerStartTag == EntombedGameInstance->CheckpointTag)
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

void AEntombedGameModeBase::HandlePlayerDeath(const ACharacter* Character)
{
	UEntombedSaveGame* SaveGame = GetGameSaveData();
	if (!IsValid(SaveGame)) return;
	
	UGameplayStatics::OpenLevel(Character, FName(SaveGame->MapAssetName));
}

FString AEntombedGameModeBase::GetMapNameFromMapAssetName(const FString& MapAssetName)
{
	for (auto& Map : Maps)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName() == MapAssetName)
		{
			return Map.Key;
		}
	}
	return FString();
}

void AEntombedGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(DefaultMapName, DefaultMap);
}
