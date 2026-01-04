// Copyright Yooshley


#include "Game/EntombedSaveGame.h"

FSavedMap UEntombedSaveGame::GetSavedMapFromName(const FString& InMapName)
{
	for(const FSavedMap& Map : SavedMaps)
	{
		if (Map.MapAssetName == InMapName)
		{
			return Map;
		}
	}
	return FSavedMap();
}

bool UEntombedSaveGame::HasMap(const FString& InMapName)
{
	for(const FSavedMap& Map : SavedMaps)
	{
		if (Map.MapAssetName == InMapName)
		{
			return true;
		}
	}
	return false;
}
