// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "Game/EntombedCheckpoint.h"
#include "FloorEntrance.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API AFloorEntrance : public AEntombedCheckpoint
{
	GENERATED_BODY()
	
public:
	AFloorEntrance(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationFloor;
	
	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;
	
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
