// Copyright Yooshley


#include "AbilitySystem/EntombedAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EntombedAbilityTypes.h"
#include "EntombedGameplayTags.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilitySystem/Ability/EntombedGameplayAbility.h"
#include "AbilitySystem/Data/ArchetypeInfo.h"
#include "Engine/OverlapResult.h"
#include "Game/EntombedGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/EntombedPlayerState.h"
#include "UI/HUD/EntombedHUD.h"
#include "UI/WidgetController/EntombedWidgetController.h"

class UEntombedGameplayAbility;

bool UEntombedAbilitySystemLibrary::GetWidgetControllerParameters(const UObject* WorldContextObject,
	FWidgetControllerParameters& OutParameters, AEntombedHUD*& OutHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		OutHUD = Cast<AEntombedHUD>(PC->GetHUD());
		if (OutHUD)
		{
			AEntombedPlayerState* PS = PC->GetPlayerState<AEntombedPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			OutParameters.PlayerController = PC;
			OutParameters.PlayerState = PS;
			OutParameters.AbilitySystemComponent = ASC;
			OutParameters.AttributeSet = AS;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UEntombedAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParameters Parameters;
	if (AEntombedHUD* HUD = nullptr; GetWidgetControllerParameters(WorldContextObject, Parameters, HUD))
	{
		return HUD->GetOverlayWidgetController(Parameters);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UEntombedAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParameters Parameters;
	if (AEntombedHUD* HUD = nullptr; GetWidgetControllerParameters(WorldContextObject, Parameters, HUD))
	{
		return HUD->GetAttributeMenuWidgetController(Parameters);
	}
	return nullptr;
}

UAbilityMenuWidgetController* UEntombedAbilitySystemLibrary::GetAbilityMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParameters Parameters;
	if (AEntombedHUD* HUD = nullptr; GetWidgetControllerParameters(WorldContextObject, Parameters, HUD))
	{
		return HUD->GetAbilityMenuWidgetController(Parameters);
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

void UEntombedAbilitySystemLibrary::GrantDefaultAbilities(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC, EEntombedArchetype Archetype)
{
	UArchetypeInfo* ArchetypeInfo = GetArchetypeInfo(WorldContextObject);
	if (ArchetypeInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : ArchetypeInfo->SharedAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UEntombedGameplayAbility* EntombedAbility = Cast<UEntombedGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(EntombedAbility->AbilityInputTag);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FEntombedGameplayTags::Get().Ability_Status_Eligible);
		}
		ASC->GiveAbility(AbilitySpec);
	}
	const FEntombedArchetypeDefaultInfo& DefaultInfo = ArchetypeInfo->GetArchetypeDefaultInfo(Archetype);
	int32 Level = 1;
	if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
	{
		Level = ICombatInterface::Execute_GetCharacterLevel(ASC->GetAvatarActor());
	}
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.DefaultAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, Level);
		if (const UEntombedGameplayAbility* EntombedAbility = Cast<UEntombedGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(EntombedAbility->AbilityInputTag);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FEntombedGameplayTags::Get().Ability_Status_Equipped);
		}
		ASC->GiveAbility(AbilitySpec);
	}
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.DefaultPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, Level);
		ASC->GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

UArchetypeInfo* UEntombedAbilitySystemLibrary::GetArchetypeInfo(const UObject* WorldContextObject)
{
	const AEntombedGameModeBase* EntombedGameMode = Cast<AEntombedGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (EntombedGameMode == nullptr) return nullptr;
	return EntombedGameMode->ArchetypeInformation;
}

UAbilityInfo* UEntombedAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AEntombedGameModeBase* EntombedGameMode = Cast<AEntombedGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (EntombedGameMode == nullptr) return nullptr;
	return EntombedGameMode->AbilityInformation;
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

bool UEntombedAbilitySystemLibrary::IsDebuffed(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FEntombedGameplayEffectContext* EntombedContext = static_cast<const FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EntombedContext->IsDebuffed();
	}
	return false;
}

float UEntombedAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FEntombedGameplayEffectContext* EntombedContext = static_cast<const FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EntombedContext->GetDebuffDamage();
	}
	return 0.f;
}

float UEntombedAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FEntombedGameplayEffectContext* EntombedContext = static_cast<const FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EntombedContext->GetDebuffDuration();
	}
	return 0.f;
}

float UEntombedAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FEntombedGameplayEffectContext* EntombedContext = static_cast<const FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EntombedContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UEntombedAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FEntombedGameplayEffectContext* EntombedContext = static_cast<const FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (EntombedContext->GetDamageType().IsValid())
		{
			return *EntombedContext->GetDamageType();
		}
	}
	return FGameplayTag();
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

void UEntombedAbilitySystemLibrary::SetIsDebuffed(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsDebuffed)
{
	if (FEntombedGameplayEffectContext* EntombedContext = static_cast<FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EntombedContext->SetIsDebuffed(bInIsDebuffed);
	}
}

void UEntombedAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDamage)
{
	if (FEntombedGameplayEffectContext* EntombedContext = static_cast<FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EntombedContext->SetDebuffDamage(InDamage);
	}
}

void UEntombedAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle,
	float InDuration)
{
	if (FEntombedGameplayEffectContext* EntombedContext = static_cast<FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EntombedContext->SetDebuffDuration(InDuration);
	}
}

void UEntombedAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle,
	float InFrequency)
{
	if (FEntombedGameplayEffectContext* EntombedContext = static_cast<FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
    {
    	EntombedContext->SetDebuffFrequency(InFrequency);
    }
}

void UEntombedAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& InType)
{
	if (FEntombedGameplayEffectContext* EntombedContext = static_cast<FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
    {
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InType);
    	EntombedContext->SetDamageType(DamageType);
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

FGameplayEffectContextHandle UEntombedAbilitySystemLibrary::ApplyDamageEffect(
	const FDamageEffectParameters& DamageEffectParameters)
{
	const FEntombedGameplayTags& GameplayTags = FEntombedGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParameters.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParameters.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);

	const FGameplayEffectSpecHandle EffectSpecHandle = DamageEffectParameters.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParameters.DamageEffectClass, DamageEffectParameters.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, DamageEffectParameters.DamageType, DamageEffectParameters.DamageValue);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Debuff_Chance, DamageEffectParameters.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Debuff_Damage, DamageEffectParameters.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Debuff_Duration, DamageEffectParameters.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParameters.DebuffFrequency);
	
	DamageEffectParameters.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
	return EffectContextHandle;
}

int32 UEntombedAbilitySystemLibrary::GetXPAwardForArchetype(const UObject* WorldContextObject,
                                                            EEntombedArchetype Archetype, int32 Level)
{
	UArchetypeInfo* ArchetypeInfo = GetArchetypeInfo(WorldContextObject);
	if (ArchetypeInfo == nullptr) 0;
	
	FEntombedArchetypeDefaultInfo Info = ArchetypeInfo->GetArchetypeDefaultInfo(Archetype);
	const float XPAward = Info.XPAward.GetValueAtLevel(Level);

	return static_cast<int32>(XPAward);
}
