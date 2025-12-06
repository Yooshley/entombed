// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

class URotatingMovementComponent;

UCLASS()
class ENTOMBED_API AMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	AMagicCircle();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> DecalComponent;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<URotatingMovementComponent> RotatingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationRate = 90.f;
};
