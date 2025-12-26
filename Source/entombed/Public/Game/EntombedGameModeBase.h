// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EntombedGameModeBase.generated.h"

class UEntombedSaveGame;
class USaveGame;
class UMVVM_Slot;
class UAbilityInfo;
class UArchetypeInfo;

/**
 * 
 */
UCLASS()
class ENTOMBED_API AEntombedGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Archetype")
	TObjectPtr<UArchetypeInfo> ArchetypeInformation;

	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TObjectPtr<UAbilityInfo> AbilityInformation;

	static void DeleteSlot(UMVVM_Slot* Slot, int32 SlotIndex);
	void SaveSlot(UMVVM_Slot* Slot, int32 SlotIndex);
	UEntombedSaveGame* GetSlotData(const FString& SlotName, int32 SlotIndex);
	void TravelToMap(UMVVM_Slot* Slot);
	
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> SaveGameClass;
	
	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;
	
	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;
	
protected:
	virtual void BeginPlay() override;
};
