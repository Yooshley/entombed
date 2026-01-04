// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Data/ArchetypeInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EntombedAbilitySystemLibrary.generated.h"

class ULootTiers;
class UEntombedSaveGame;
struct FDamageEffectParameters;
class UAbilityInfo;
struct FWidgetControllerParameters;
class UAbilityMenuWidgetController;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/*
	 * WIDGET CONTROLLER
	 */
	
	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static bool GetWidgetControllerParameters(const UObject* WorldContextObject, FWidgetControllerParameters& OutParameters, AEntombedHUD*& OutHUD);
	
	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static UAbilityMenuWidgetController* GetAbilityMenuWidgetController(const UObject* WorldContextObject);

	/*
	 * ABILITY SYSTEM DEFAULTS
	 */

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|ArchetypeDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, EEntombedArchetype Archetype, float Level, UAbilitySystemComponent* ASC);
	
	static void InitializeAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, UEntombedSaveGame* SaveData);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|ArchetypeDefaults")
	static void GrantDefaultAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, EEntombedArchetype Archetype);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|ArchetypeDefaults")
	static UArchetypeInfo* GetArchetypeInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|ArchetypeDefaults")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|ArchetypeDefaults", meta=(DefaultToSelf = "WorldContextObject"))
	static ULootTiers* GetLootTiers(const UObject* WorldContextObject);

	/*
	 * EFFECT CONTEXT GETTERS
	 */
	
	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static bool IsDebuffed(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static FVector GetKnockbackVector(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static bool GetIsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle);

	/*
	* EFFECT CONTEXT SETTERS
	*/

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static void SetIsDebuffed(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsDebuffed);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDamage);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDuration);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InFrequency);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InType);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static void SetKnockbackVector(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InVector);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static void SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageInnerRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InRadius);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOuterRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InRadius);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOrigin(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InOrigin);

	/*
	 * GAMEPLAY MECHANICS
	 */

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayMechanics")
	static void GetLiveActorsWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& Origin);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayMechanics")
	static void GetClosestActors(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestActors, const FVector& Origin);
	
	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayMechanics")
	static bool IsAlly(AActor* FirstActor, AActor* SecondActor);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayMechanics")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParameters& DamageEffectParameters);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayMechanics")
	static TArray<FRotator> GetEvenlySpreadRotators(const FVector Forward, const FVector Axis, float Spread, int32 NumRotators);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayMechanics")
	static TArray<FVector> GetEvenlySpreadVectors(const FVector Forward, const FVector Axis, float Spread, int32 NumVectors);

	static int32 GetXPAwardForArchetype(const UObject* WorldContextObject, EEntombedArchetype Archetype, int32 Level);

	/*
	 * DAMAGE PARAMETERS
	 */

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|DamageEffect")
	static void SetRadialDamageEffectParams(UPARAM(ref) FDamageEffectParameters& DamageEffectParameters);
};