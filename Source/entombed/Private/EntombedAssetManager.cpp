// Copyright Yooshley


#include "EntombedAssetManager.h"
#include "EntombedGameplayTags.h"
#include "AbilitySystemGlobals.h"

UEntombedAssetManager& UEntombedAssetManager::Get()
{
	check(GEngine);
	
	UEntombedAssetManager* EntombedAssetManager = Cast<UEntombedAssetManager>(GEngine->AssetManager);
	return *EntombedAssetManager;
}

void UEntombedAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FEntombedGameplayTags::InitializeNativeGameplayTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
