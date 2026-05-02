// Copyright Yooshley


#include "AbilitySystem/EntombedAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EntombedAbilityTypes.h"
#include "EntombedGameplayTags.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilitySystem/Ability/EntombedAbility.h"
#include "AbilitySystem/Data/ArchetypeInfo.h"
#include "Engine/OverlapResult.h"
#include "Game/EntombedGameModeBase.h"
#include "entombed/Public/Interface/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/EntombedPlayerState.h"
#include "UI/HUD/EntombedHUD.h"
#include "UI/WidgetController/EntombedWidgetController.h"

class UEntombedAbility;

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
	if (ArchetypeInfo == nullptr) return;
	FEntombedArchetypeDefaultInfo ArchetypeDefaultInfo = ArchetypeInfo->GetArchetypeDefaultInfo(Archetype);
	
	FGameplayEffectContextHandle AttributesContextHandle = ASC->MakeEffectContext();
	AttributesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle AttributesSpecHandle = ASC->MakeOutgoingSpec(ArchetypeDefaultInfo.DefaultAttributes, Level, AttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*AttributesSpecHandle.Data.Get());
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

	const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(ArchetypeInfo->Attributes_SetByCaller, 1.f, EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
}

void UEntombedAbilitySystemLibrary::GrantDefaultAbilities(const UObject* WorldContextObject,
                                                          UAbilitySystemComponent* ASC, EEntombedArchetype Archetype)
{
	UArchetypeInfo* ArchetypeInfo = GetArchetypeInfo(WorldContextObject);
	if (ArchetypeInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : ArchetypeInfo->SharedAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UEntombedAbility* EntombedAbility = Cast<UEntombedAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(EntombedAbility->AbilityActivationTag);
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
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.DefaultActiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, Level);
		if (const UEntombedAbility* EntombedAbility = Cast<UEntombedAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(EntombedAbility->AbilityActivationTag);
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

bool UEntombedAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FEntombedGameplayEffectContext* EntombedContext = static_cast<const FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EntombedContext->IsCriticalHit();
	}
	return false;
}

bool UEntombedAbilitySystemLibrary::HasDebuffs(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FEntombedGameplayEffectContext* EntombedContext = static_cast<const FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EntombedContext->HasDebuffs();
	}
	return false;
}

TArray<FAppliedDebuffData> UEntombedAbilitySystemLibrary::GetAppliedDebuffs(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FEntombedGameplayEffectContext* EntombedContext = static_cast<const FEntombedGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return EntombedContext->GetAppliedDebuffs();
	}
	return {};
}

void UEntombedAbilitySystemLibrary::AddDebuff(const FGameplayEffectContextHandle& EffectContextHandle, const FAppliedDebuffData& InDebuff)
{
	if (const FGameplayEffectContext* BaseContext = EffectContextHandle.Get())
	{
		if (FEntombedGameplayEffectContext* EntombedContext = static_cast<FEntombedGameplayEffectContext*>(const_cast<FGameplayEffectContext*>(BaseContext)))
		{
			EntombedContext->AddDebuff(InDebuff);
		}
	}
}

void UEntombedAbilitySystemLibrary::ClearDebuffs(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FGameplayEffectContext* BaseContext = EffectContextHandle.Get())
	{
		if (FEntombedGameplayEffectContext* EntombedContext = static_cast<FEntombedGameplayEffectContext*>(const_cast<FGameplayEffectContext*>(BaseContext)))
		{
			EntombedContext->ClearDebuffs();
		}
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
	const FAbilityDamageParameters& DamageEffectParameters)
{
	const FEntombedGameplayTags& GameplayTags = FEntombedGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParameters.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParameters.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);

	const FGameplayEffectSpecHandle EffectSpecHandle = DamageEffectParameters.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParameters.DamageEffectClass, DamageEffectParameters.AbilityLevel, EffectContextHandle);
	
	for (FEntombedDamageData DamageType : DamageEffectParameters.DamageTypes)
	{
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
            EffectSpecHandle,
            DamageType.Tag,
            DamageType.Value.GetValueAtLevel(DamageEffectParameters.AbilityLevel)
        );
    
        //Debuff Chance
		if (const FGameplayTag* ChanceTag = GameplayTags.DamageTypesToDebuffChances.Find(DamageType.Tag))
        {
            UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
                EffectSpecHandle,
                *ChanceTag,
                DamageType.Debuff.Chance.GetValueAtLevel(DamageEffectParameters.AbilityLevel)
            );
        }
    
        //Debuff Damage
		if (const FGameplayTag* DamageTag = GameplayTags.DamageTypesToDebuffDamages.Find(DamageType.Tag))
        {
            UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
                EffectSpecHandle,
                *DamageTag,
                DamageType.Debuff.Damage.GetValueAtLevel(DamageEffectParameters.AbilityLevel)
            );
        }
    
        //Debuff Duration
		if (const FGameplayTag* DurationTag = GameplayTags.DamageTypesToDebuffDurations.Find(DamageType.Tag))
        {
            UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
                EffectSpecHandle,
                *DurationTag,
                DamageType.Debuff.Duration.GetValueAtLevel(DamageEffectParameters.AbilityLevel)
            );
        }
		
		//Debuff Period
		if (const FGameplayTag* PeriodTag = GameplayTags.DamageTypesToDebuffPeriods.Find(DamageType.Tag))
		{
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
				EffectSpecHandle,
				*PeriodTag,
				DamageType.Debuff.Period.GetValueAtLevel(DamageEffectParameters.AbilityLevel)
			);
		}
		
		//Knockback
		if (DamageType.bCanKnockback)
		{
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
				EffectSpecHandle,
				GameplayTags.Effect_Knockback,
				DamageType.Force
			);
		}
	}
	
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
	if (ArchetypeInfo == nullptr) return 0;
	
	FEntombedArchetypeDefaultInfo Info = ArchetypeInfo->GetArchetypeDefaultInfo(Archetype);
	const float XPAward = Info.XPAward.GetValueAtLevel(Level);

	return static_cast<int32>(XPAward);
}
