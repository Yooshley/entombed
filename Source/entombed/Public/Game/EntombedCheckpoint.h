// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interface/SaveInterface.h"
#include "Interface/TargetInterface.h"
#include "EntombedCheckpoint.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class ENTOMBED_API AEntombedCheckpoint : public APlayerStart, public ISaveInterface, public ITargetInterface
{
	GENERATED_BODY()
	
public:
	AEntombedCheckpoint(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bActivated = false;
	
	UPROPERTY(EditAnywhere)
	bool bBindOverlapCallback = true;
	
	/* Save Interface Begin */
	virtual bool ShouldLoadTransform_Implementation() override { return false; }
	virtual void LoadActor_Implementation() override;
	/* Save Interface End */
	
	/* Target Interface Begin */
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	/* Target Interface End */
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UStaticMeshComponent> CheckpointMesh;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
};
