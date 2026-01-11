// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "EntombedAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "EntombedAbilityProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class ENTOMBED_API AEntombedAbilityProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AEntombedAbilityProjectile();
	
	void InitializeProjectile(const FAbilityProjectileParameters& InParameters);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;
	
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn=true))
	FAbilityDamageParameters DamageParameters;
	
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn=true))
	FAbilityProjectileParameters ProjectileParameters;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnHit();

	bool bHit = false;
	
	bool bHitMultiple = false;
	
	TSet<TWeakObjectPtr<AActor>> HitActors;
	
	virtual void Destroyed() override;
	bool IsValidOverlap(AActor* OtherActor) const;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComponent;
	
	UFUNCTION()
	void OnHitMeshOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 BodyIndex,bool bFromSweep,const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category="Collision")
	UStaticMeshComponent* HitMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> TraversalEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> TraversalSound;
};
