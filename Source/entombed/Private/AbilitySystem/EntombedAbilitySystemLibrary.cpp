// Copyright Yooshley


#include "AbilitySystem/EntombedAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/EntombedPlayerState.h"
#include "UI/HUD/EntombedHUD.h"
#include "UI/WidgetController/EntombedWidgetController.h"

UOverlayWidgetController* UEntombedAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AEntombedHUD* EntombedHUD = Cast<AEntombedHUD>(PC->GetHUD()))
		{
			AEntombedPlayerState* PS = PC->GetPlayerState<AEntombedPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParameters WidgetControllerParameters(PC, PS, ASC, AS);
			return EntombedHUD->GetOverlayWidgetController(WidgetControllerParameters);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UEntombedAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AEntombedHUD* EntombedHUD = Cast<AEntombedHUD>(PC->GetHUD()))
		{
			AEntombedPlayerState* PS = PC->GetPlayerState<AEntombedPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParameters WidgetControllerParameters(PC, PS, ASC, AS);
			return EntombedHUD->GetAttributeMenuWidgetController(WidgetControllerParameters);
		}
	}
	return nullptr;
}
