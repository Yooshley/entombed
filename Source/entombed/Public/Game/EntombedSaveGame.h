// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "EntombedSaveGame.generated.h"

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	Name,
	Taken
};

/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FString SlotName = FString();
	
	UPROPERTY()
	int32 SlotIndex = 0;
	
	UPROPERTY()
	FString SavedName = FString("Default");
	
	UPROPERTY()
	FString MapName = FString("Default");
	
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant;
};
