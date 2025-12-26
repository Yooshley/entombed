// Copyright Yooshley


#include "UI/HUD/MainMenuHUD.h"

#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/ViewModel/MVVM_MainMenu.h"
#include "UI/Widget/MainMenuWidget.h"

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	
	MainMenuViewModel = NewObject<UMVVM_MainMenu>(this, MainMenuViewModelClass);
	MainMenuViewModel->InitializeSlots();
	
	MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayerController(), MainMenuWidget, EMouseLockMode::DoNotLock, false);
	GetOwningPlayerController()->bShowMouseCursor = true;
}
