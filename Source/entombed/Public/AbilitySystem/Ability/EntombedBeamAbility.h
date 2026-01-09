// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/EntombedDamageAbility.h"
#include "EntombedBeamAbility.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API UEntombedBeamAbility : public UEntombedDamageAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void StoreCursorDataInfo(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();

	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);

	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);

	UFUNCTION(BlueprintImplementableEvent)
	void PrimaryTargetDead(AActor* DeadActor);

	UFUNCTION(BlueprintImplementableEvent)
	void AdditionalTargetDead(AActor* DeadActor);
	
protected:
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	FVector CursorHitLocation;

	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<AActor> CursorHitActor;

	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;

	UPROPERTY(EditDefaultsOnly, Category="Beam")
	int32 MaxChainTargets = 5;
};
