// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "EntombedAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	static UEntombedAssetManager& Get();
	
protected:
	virtual void StartInitialLoading() override;
	
};
