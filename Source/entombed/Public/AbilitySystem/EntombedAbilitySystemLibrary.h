// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Data/ArchetypeInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EntombedAbilitySystemLibrary.generated.h"

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
	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static bool GetWidgetControllerParameters(const UObject* WorldContextObject, FWidgetControllerParameters& OutParameters, AEntombedHUD*& OutHUD);
	
	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static UAbilityMenuWidgetController* GetAbilityMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|ArchetypeDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, EEntombedArchetype Archetype, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|ArchetypeDefaults")
	static void GrantDefaultAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, EEntombedArchetype Archetype);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|ArchetypeDefaults")
	static UArchetypeInfo* GetArchetypeInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|ArchetypeDefaults")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);
	
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

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& Origin);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayMechanics")
	static bool IsAlly(AActor* FirstActor, AActor* SecondActor);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayMechanics")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParameters& DamageEffectParameters);

	static int32 GetXPAwardForArchetype(const UObject* WorldContextObject, EEntombedArchetype Archetype, int32 Level);
};
