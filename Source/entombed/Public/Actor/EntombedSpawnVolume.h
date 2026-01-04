// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SaveInterface.h"
#include "EntombedSpawnVolume.generated.h"

class AEntombedSpawnPoint;
class UBoxComponent;

UCLASS()
class ENTOMBED_API AEntombedSpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	AEntombedSpawnVolume();
	
	/* Save Interface Begin */
	virtual void LoadActor_Implementation() override;
	/* Save Interface End */
	
	UPROPERTY(BlueprintReadOnly)
	bool bActivated = false;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere)
	TArray<AEntombedSpawnPoint*> SpawnPoints;
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;
};
