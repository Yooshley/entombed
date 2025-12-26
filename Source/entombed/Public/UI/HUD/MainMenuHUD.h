// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

class UMVVM_MainMenu;
class UMainMenuWidget;
/**
 * 
 */
UCLASS()
class ENTOMBED_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMainMenuWidget> MainMenuWidget;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_MainMenu> MainMenuViewModelClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_MainMenu> MainMenuViewModel;
	
protected:
	virtual void BeginPlay() override;
};
