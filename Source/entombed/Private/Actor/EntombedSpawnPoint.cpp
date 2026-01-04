// Copyright Yooshley


#include "Actor/EntombedSpawnPoint.h"

#include "Character/EntombedEnemyCharacter.h"

void AEntombedSpawnPoint::Spawn()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	AEntombedEnemyCharacter* Enemy = GetWorld()->SpawnActorDeferred<AEntombedEnemyCharacter>(SpawnClass, GetActorTransform());
	if (Enemy->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_SetCharacterLevel(Enemy, Level);
		ICombatInterface::Execute_SetArchetype(Enemy, Archetype);
	}
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
}
