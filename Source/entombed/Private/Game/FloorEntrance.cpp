// Copyright Yooshley


#include "Game/FloorEntrance.h"

#include "Game/EntombedGameModeBase.h"
#include "Interface/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

AFloorEntrance::AFloorEntrance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void AFloorEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
    {
    	if (AEntombedGameModeBase* EntombedGameMode = Cast<AEntombedGameModeBase>(UGameplayStatics::GetGameMode(this)))
    	{
    		EntombedGameMode->SaveWorldState(GetWorld(), DestinationFloor.ToSoftObjectPath().GetAssetName());
    	}
    	IPlayerInterface::Execute_SavePlayerProgress(OtherActor, DestinationPlayerStartTag);
		
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationFloor);
    }
}
