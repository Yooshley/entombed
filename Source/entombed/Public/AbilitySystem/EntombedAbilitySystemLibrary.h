// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Data/ArchetypeInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EntombedAbilitySystemLibrary.generated.h"

struct FAppliedDebuffData;
class ULootTiers;
class UEntombedSaveGame;
struct FAbilityDamageParameters;
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
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static bool HasDebuffs(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static TArray<FAppliedDebuffData> GetAppliedDebuffs(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static void AddDebuff(const FGameplayEffectContextHandle& EffectContextHandle, const FAppliedDebuffData& InDebuff);
	
	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static void ClearDebuffs(const FGameplayEffectContextHandle& EffectContextHandle);

	/*
	* EFFECT CONTEXT SETTERS
	*/

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);
	

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
	static FGameplayEffectContextHandle ApplyDamageEffect(const FAbilityDamageParameters& DamageEffectParameters);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayMechanics")
	static TArray<FRotator> GetEvenlySpreadRotators(const FVector Forward, const FVector Axis, float Spread, int32 NumRotators);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayMechanics")
	static TArray<FVector> GetEvenlySpreadVectors(const FVector Forward, const FVector Axis, float Spread, int32 NumVectors);

	static int32 GetXPAwardForArchetype(const UObject* WorldContextObject, EEntombedArchetype Archetype, int32 Level);
};