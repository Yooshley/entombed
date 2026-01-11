// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "Actor/EntombedAbilityProjectile.h"
#include "EntombedFireBall.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API AEntombedFireBall : public AEntombedAbilityProjectile
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeline();
	
	UPROPERTY(BlueprintReadWrite)
	FAbilityDamageParameters ExplosionDamageParams;

protected:
	virtual void BeginPlay() override;
	//virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnHit() override;
};
