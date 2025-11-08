// Copyright Yooshley


#include "AbilitySystem/EntombedAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "EntombedAbilityTypes.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilitySystem/Ability/EntombedGameplayAbility.h"
#include "AbilitySystem/Data/ArchetypeInfo.h"
#include "Game/EntombedGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/EntombedPlayerState.h"
#include "UI/HUD/EntombedHUD.h"
#include "UI/WidgetController/EntombedWidgetController.h"

class UEntombedGameplayAbility;

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

void UEntombedAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, EEntombedArchetype Archetype, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();
	
	UArchetypeInfo* ProfInfo = GetArchetypeInfo(WorldContextObject);
	FEntombedArchetypeDefaultInfo ProfDefaultInfo = ProfInfo->GetArchetypeDefaultInfo(Archetype);
	
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
	UAbilitySystemComponent* ASC, EEntombedArchetype Archetype)
{
	UArchetypeInfo* ArchetypeInfo = GetArchetypeInfo(WorldContextObject);
	if (ArchetypeInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : ArchetypeInfo->SharedAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UEntombedGameplayAbility* EntombedAbility = Cast<UEntombedGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(EntombedAbility->StartupInputTag);
		}
		ASC->GiveAbility(AbilitySpec);
	}
	const FEntombedArchetypeDefaultInfo& DefaultInfo = ArchetypeInfo->GetArchetypeDefaultInfo(Archetype);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetCharacterLevel());
			if (const UEntombedGameplayAbility* EntombedAbility = Cast<UEntombedGameplayAbility>(AbilitySpec.Ability))
			{
				AbilitySpec.GetDynamicSpecSourceTags().AddTag(EntombedAbility->StartupInputTag);
			}
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UArchetypeInfo* UEntombedAbilitySystemLibrary::GetArchetypeInfo(const UObject* WorldContextObject)
{
	AEntombedGameModeBase* EntombedGameMode = Cast<AEntombedGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (EntombedGameMode == nullptr) return nullptr;
	return EntombedGameMode->ArchetypeInformation;
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

void UEntombedAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& Origin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> OverlapResults;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(OverlapResults, Origin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& OverlapResult : OverlapResults)
		{
			const bool bHasCombatInterface = OverlapResult.GetActor()->Implements<UCombatInterface>();
			if (bHasCombatInterface)
			{
				const bool bIsDead = ICombatInterface::Execute_IsDead(OverlapResult.GetActor());
				if (!bIsDead)
				{
					OutOverlappingActors.AddUnique(OverlapResult.GetActor());
				}
			}
		}
	}
}

bool UEntombedAbilitySystemLibrary::IsAlly(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	return bBothArePlayers || bBothAreEnemies;
}
