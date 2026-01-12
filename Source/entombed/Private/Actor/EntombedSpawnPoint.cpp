// Copyright Yooshley


#include "Actor/EntombedSpawnPoint.h"

#include "Character/EntombedHostileCharacter.h"

void AEntombedSpawnPoint::Spawn()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	AEntombedHostileCharacter* Enemy = GetWorld()->SpawnActorDeferred<AEntombedHostileCharacter>(SpawnClass, GetActorTransform());
	if (Enemy->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_SetCharacterLevel(Enemy, Level);
		ICombatInterface::Execute_SetArchetype(Enemy, Archetype);
	}
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
}
