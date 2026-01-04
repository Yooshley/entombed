// Copyright Yooshley


#include "Game/EntombedCheckpoint.h"

#include "Components/SphereComponent.h"
#include "entombed/entombed.h"
#include "Game/EntombedGameModeBase.h"
#include "Interface/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

AEntombedCheckpoint::AEntombedCheckpoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	
	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckpointMesh");
	CheckpointMesh->SetupAttachment(GetRootComponent());
	CheckpointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckpointMesh->SetCollisionResponseToAllChannels(ECR_Block);
	
	CheckpointMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_WHITE);
	CheckpointMesh->MarkRenderStateDirty();
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
    Sphere->SetupAttachment(CheckpointMesh);
    Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AEntombedCheckpoint::LoadActor_Implementation()
{
	if (bActivated)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEntombedCheckpoint::HighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(true);
}

void AEntombedCheckpoint::UnHighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(false);
}

void AEntombedCheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
	if (bBindOverlapCallback)
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &AEntombedCheckpoint::OnSphereOverlap);
	}
}

void AEntombedCheckpoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bActivated = true;
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		
		if (AEntombedGameModeBase* EntombedGameMode = Cast<AEntombedGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			const UWorld* WorldObject = GetWorld();
			FString MapName = WorldObject->GetMapName();
			MapName.RemoveFromStart(WorldObject->StreamingLevelsPrefix);
			
			EntombedGameMode->SaveWorldState(GetWorld(), MapName);
		}
		IPlayerInterface::Execute_SavePlayerProgress(OtherActor, PlayerStartTag);
	}
}
