// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "EntombedAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "EntombedAbilityActor.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class ENTOMBED_API AEntombedAbilityActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEntombedAbilityActor();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn=true))
	FAbilityDamageParameters DamageEffectParameters;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnHit();

	bool bHit = false;
	
	virtual void Destroyed() override;
	bool IsValidOverlap(AActor* OtherActor);
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> FlightAudioComponent;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> FlightSound;
};
