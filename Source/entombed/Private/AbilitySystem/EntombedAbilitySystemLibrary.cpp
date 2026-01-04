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
#include "entombed/Public/Interface/CombatInterface.h"
#include "Game/EntombedSaveGame.h"
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
	
	UArchetypeInfo* ArchetypeInfo = GetArchetypeInfo(WorldContextObject);
	FEntombedArchetypeDefaultInfo ArchetypeDefaultInfo = ArchetypeInfo->GetArchetypeDefaultInfo(Archetype);
	
	FGameplayEffectContextHandle CoreAttributesContextHandle = ASC->MakeEffectContext();
	CoreAttributesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle CoreAttributesSpecHandle = ASC->MakeOutgoingSpec(ArchetypeDefaultInfo.CoreAttributesEffect, Level, CoreAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*CoreAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle DerivedAttributesContextHandle = ASC->MakeEffectContext();
	DerivedAttributesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle DerivedAttributesSpecHandle = ASC->MakeOutgoingSpec(ArchetypeInfo->DerivedAttributesEffect, Level, DerivedAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*DerivedAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle ResourceAttributesContextHandle = ASC->MakeEffectContext();
	ResourceAttributesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle ResourceAttributesSpecHandle = ASC->MakeOutgoingSpec(ArchetypeInfo->ResourceAttributesEffect, Level, ResourceAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*ResourceAttributesSpecHandle.Data.Get());
}

void UEntombedAbilitySystemLibrary::InitializeAttributesFromSaveData(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC, UEntombedSaveGame* SaveData)
{
	UArchetypeInfo* ArchetypeInfo = GetArchetypeInfo(WorldContextObject);
	if (ArchetypeInfo == nullptr) return;
	
	const FEntombedGameplayTags& GameplayTags = FEntombedGameplayTags::Get();
	const AActor* SourceAvatarActor = ASC->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);

	const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(ArchetypeInfo->CoreAttributes_SetByCaller, 1.f, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Attribute_Core_Vigor, SaveData->Vigor);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Attribute_Core_Instinct, SaveData->Instinct);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Attribute_Core_Technique, SaveData->Technique);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Attribute_Core_Acumen, SaveData->Acumen);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Attribute_Core_Logic, SaveData->Logic);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Attribute_Core_Spirit, SaveData->Spirit);
	
	ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
	
	FGameplayEffectContextHandle DerivedAttributesContextHandle = ASC->MakeEffectContext();
	DerivedAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	FGameplayEffectSpecHandle DerivedAttributesSpecHandle = ASC->MakeOutgoingSpec(ArchetypeInfo->DerivedAttributesEffect_Infinite, 1.f, DerivedAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*DerivedAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle ResourceAttributesContextHandle = ASC->MakeEffectContext();
	ResourceAttributesContextHandle.AddSourceObject(SourceAvatarActor);
	FGameplayEffectSpecHandle ResourceAttributesSpecHandle = ASC->MakeOutgoingSpec(ArchetypeInfo->ResourceAttributesEffect, 1.f, ResourceAttributesContextHandle);
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
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(FEntombedGameplayTags::Get().Ability_Status_Equipped);
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

ULootTiers* UEntombedAbilitySystemLibrary::GetLootTiers(const UObject* WorldContextObject)
{
		const AEntombedGameModeBase* EntombedGameMode = Cast<AEntombedGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
    	if (EntombedGameMode == nullptr) return nullptr;
    	return EntombedGameMode->LootTiers;
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

FVector UEntombedAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FEntombedGameplayEffectContext* EntombedContext = static_cast<const FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EntombedContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UEntombedAbilitySystemLibrary::GetKnockbackVector(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FEntombedGameplayEffectContext* EntombedContext = static_cast<const FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EntombedContext->GetKnockbackVector();
	}
	return FVector::ZeroVector;
}

bool UEntombedAbilitySystemLibrary::GetIsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FEntombedGameplayEffectContext* EntombedContext = static_cast<const FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EntombedContext->GetIsRadialDamage();
	}
	return false;
}

float UEntombedAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FEntombedGameplayEffectContext* EntombedContext = static_cast<const FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EntombedContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UEntombedAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FEntombedGameplayEffectContext* EntombedContext = static_cast<const FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EntombedContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UEntombedAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FEntombedGameplayEffectContext* EntombedContext = static_cast<const FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EntombedContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
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

void UEntombedAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InImpulse)
{
	if (FEntombedGameplayEffectContext* EntombedContext = static_cast<FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EntombedContext->SetDeathImpulse(InImpulse);
	}
}

void UEntombedAbilitySystemLibrary::SetKnockbackVector(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InVector)
{
	if (FEntombedGameplayEffectContext* EntombedContext = static_cast<FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EntombedContext->SetKnockbackVector(InVector);
	}
}


void UEntombedAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsRadialDamage)
{
	if (FEntombedGameplayEffectContext* EntombedContext = static_cast<FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EntombedContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void UEntombedAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InRadius)
{
	if (FEntombedGameplayEffectContext* EntombedContext = static_cast<FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EntombedContext->SetRadialDamageInnerRadius(InRadius);
	}
}

void UEntombedAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InRadius)
{
	if (FEntombedGameplayEffectContext* EntombedContext = static_cast<FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EntombedContext->SetRadialDamageOuterRadius(InRadius);
	}
}

void UEntombedAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InOrigin)
{
	if (FEntombedGameplayEffectContext* EntombedContext = static_cast<FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		EntombedContext->SetRadialDamageOrigin(InOrigin);
	}
}

void UEntombedAbilitySystemLibrary::GetLiveActorsWithinRadius(const UObject* WorldContextObject,
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

void UEntombedAbilitySystemLibrary::GetClosestActors(int32 MaxTargets, const TArray<AActor*>& Actors,
	TArray<AActor*>& OutClosestActors, const FVector& Origin)
{
	if (MaxTargets < 1) return;
 
	OutClosestActors = Actors;
	Algo::Sort(OutClosestActors, [&Origin](AActor* A, AActor* B)
	{
		// NOTE: Calculating the squared distance
		const float DistanceA = FVector::DistSquared(A->GetActorLocation(), Origin);
		const float DistanceB = FVector::DistSquared(B->GetActorLocation(), Origin);
		return DistanceA < DistanceB;
	});
	if (OutClosestActors.Num() > MaxTargets)
	{
		// Remove unwanted actors from the end of the array
		OutClosestActors.RemoveAt(MaxTargets, OutClosestActors.Num() - MaxTargets);
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
	SetDeathImpulse(EffectContextHandle, DamageEffectParameters.DeathImpulse);
	SetKnockbackVector(EffectContextHandle, DamageEffectParameters.KnockbackVector);
	
	SetIsRadialDamage(EffectContextHandle, DamageEffectParameters.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, DamageEffectParameters.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, DamageEffectParameters.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, DamageEffectParameters.RadialDamageOrigin);

	const FGameplayEffectSpecHandle EffectSpecHandle = DamageEffectParameters.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParameters.DamageEffectClass, DamageEffectParameters.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, DamageEffectParameters.DamageType, DamageEffectParameters.DamageValue);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Debuff_Chance, DamageEffectParameters.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Debuff_Damage, DamageEffectParameters.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Debuff_Duration, DamageEffectParameters.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParameters.DebuffFrequency);
	
	DamageEffectParameters.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
	return EffectContextHandle;
}

TArray<FRotator> UEntombedAbilitySystemLibrary::GetEvenlySpreadRotators(const FVector Forward, const FVector Axis,
	float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;
		
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread/2, Axis);
	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, Axis);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}

	return Rotators;
}

TArray<FVector> UEntombedAbilitySystemLibrary::GetEvenlySpreadVectors(const FVector Forward, const FVector Axis,
	float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;
		
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread/2, Axis);
	if (NumVectors > 1)
	{
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, Axis);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}

	return Vectors;
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

void UEntombedAbilitySystemLibrary::SetRadialDamageEffectParams(FDamageEffectParameters& DamageEffectParameters)
{
	DamageEffectParameters.bIsRadialDamage = true;
}
