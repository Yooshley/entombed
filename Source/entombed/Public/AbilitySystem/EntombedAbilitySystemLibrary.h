// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "Data/ProfessionInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EntombedAbilitySystemLibrary.generated.h"

struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
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
	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|ProfessionDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, EProfession Profession, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|ProfessionDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|ProfessionDefaults")
	static UProfessionInfo* GetProfessionInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "EntombedAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);
};
