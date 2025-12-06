// Copyright Yooshley


#include "Actor/MagicCircle.h"

#include "GameFramework/RotatingMovementComponent.h"
#include "Components/DecalComponent.h"

AMagicCircle::AMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;

	DecalComponent = CreateDefaultSubobject<UDecalComponent>("DecalComponent");
	DecalComponent->SetupAttachment(GetRootComponent());

	RotatingComponent = CreateDefaultSubobject<URotatingMovementComponent>("RotatingMovement");
	RotatingComponent->RotationRate = FRotator(0.f, 0.f, RotationRate);
	RotatingComponent->SetUpdatedComponent(DecalComponent);
}

void AMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}


