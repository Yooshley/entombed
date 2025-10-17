// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EntombedEffectActor.generated.h"

class USphereComponent;

UCLASS()
class ENTOMBED_API AEntombedEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEntombedEffectActor();

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
};
