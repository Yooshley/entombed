// Copyright Yooshley


#include "Actor/EntombedSpawnVolume.h"

#include "Actor/EntombedSpawnPoint.h"
#include "Components/BoxComponent.h"
#include "Interface/PlayerInterface.h"

AEntombedSpawnVolume::AEntombedSpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	SetRootComponent(Box);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionObjectType(ECC_WorldStatic);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AEntombedSpawnVolume::LoadActor_Implementation()
{
	if(bActivated)
	{
		Destroy();
	}
}

void AEntombedSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
	Box->OnComponentBeginOverlap.AddDynamic(this, &AEntombedSpawnVolume::OnBoxOverlap);
}

void AEntombedSpawnVolume::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UPlayerInterface>()) return;
	
	bActivated = true;
	for (AEntombedSpawnPoint* Point : SpawnPoints)
	{
		if (IsValid(Point))
		{
			Point->Spawn();
		}
	}
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

