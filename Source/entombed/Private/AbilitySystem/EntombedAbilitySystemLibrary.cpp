// Copyright Yooshley


#include "AbilitySystem/EntombedAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "EntombedAbilityTypes.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilitySystem/Data/ProfessionInfo.h"
#include "Game/EntombedGameModeBase.h"
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

void UEntombedAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, EProfession Profession, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();
	
	UProfessionInfo* ProfInfo = GetProfessionInfo(WorldContextObject);
	FProfessionDefaultInfo ProfDefaultInfo = ProfInfo->GetProfessionDefaultInfo(Profession);
	
	FGameplayEffectContextHandle CoreAttributesContextHandle = ASC->MakeEffectContext();
	CoreAttributesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle CoreAttributesSpecHandle = ASC->MakeOutgoingSpec(ProfDefaultInfo.CoreAttributesEffect, Level, CoreAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*CoreAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle DerivedAttributesContextHandle = ASC->MakeEffectContext();
	CoreAttributesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle DerivedAttributesSpecHandle = ASC->MakeOutgoingSpec(ProfInfo->DerivedAttributesEffect, Level, DerivedAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*DerivedAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle ResourceAttributesContextHandle = ASC->MakeEffectContext();
	CoreAttributesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle ResourceAttributesSpecHandle = ASC->MakeOutgoingSpec(ProfInfo->ResourceAttributesEffect, Level, ResourceAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*ResourceAttributesSpecHandle.Data.Get());
}

void UEntombedAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC)
{
	UProfessionInfo* ProfInfo = GetProfessionInfo(WorldContextObject);
	for (TSubclassOf<UGameplayAbility> AbilityClass : ProfInfo->SharedAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
}

UProfessionInfo* UEntombedAbilitySystemLibrary::GetProfessionInfo(const UObject* WorldContextObject)
{
	AEntombedGameModeBase* EntombedGameMode = Cast<AEntombedGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (EntombedGameMode == nullptr) return nullptr;
	return EntombedGameMode->ProfessionInformation;
}

bool UEntombedAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FEntombedGameplayEffectContext* EntombedContext = static_cast<const FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EntombedContext->IsBlockedHit();
	}
	return false;
}

bool UEntombedAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FEntombedGameplayEffectContext* EntombedContext = static_cast<const FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EntombedContext->IsCriticalHit();
	}
	return false;
}

void UEntombedAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsBlockedHit)
{
	if (FEntombedGameplayEffectContext* EntombedContext = static_cast<FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EntombedContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UEntombedAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	if (FEntombedGameplayEffectContext* EntombedContext = static_cast<FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EntombedContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}
