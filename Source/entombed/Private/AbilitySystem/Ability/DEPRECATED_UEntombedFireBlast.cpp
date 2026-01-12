// Copyright Yooshley


#include "AbilitySystem/Ability/DEPRECATED_UEntombedFireBlast.h"

#include "AbilitySystem/EntombedAbilitySystemLibrary.h"
#include "Actor/ADEPRECATED_AEntombedFireBall.h"

FString UDEPRECATED_UEntombedFireBlast::GetLevelDescription(int32 Level)
{
	const int32 ScaledDamage = 0;
	const float Cost = FMath::Abs(GetCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		"<Title>FIRE BLAST</>\n\n"

		"<Small>Level: </><Level>%d</>\n"
		"<Small>Cost: </><Cost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			
		"<Default>Launches %d balls of fire, exploding on impact and dealing </>"
		"<Damage>%d Damage.</>"), NumFireBalls, Cost, Cooldown, Level, ScaledDamage);
}

FString UDEPRECATED_UEntombedFireBlast::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = 0;
	const float Cost = FMath::Abs(GetCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL</>\n\n"

		"<Small>Level: </><Level>%d</>\n"
		"<Small>Cost: </><Cost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			
		"<Default>Launches %d balls of fire, exploding on impact and dealing </>"
		"<Damage>%d Damage.</>"), NumFireBalls, Cost, Cooldown, Level, ScaledDamage);
}

TArray<ADEPRECATED_AEntombedFireBall*> UDEPRECATED_UEntombedFireBlast::SpawnFireBalls()
{
	TArray<ADEPRECATED_AEntombedFireBall*> FireBalls;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UEntombedAbilitySystemLibrary::GetEvenlySpreadRotators(Forward, FVector::UpVector, 360.f, NumFireBalls);

	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		// ADEPRECATED_AEntombedFireBall* FireBall = GetWorld()->SpawnActorDeferred<ADEPRECATED_AEntombedFireBall>(
		// 	FireBallClass,
		// 	SpawnTransform,
		// 	GetOwningActorFromActorInfo(),
		// 	CurrentActorInfo->PlayerController->GetPawn(),
		// 	ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		// 	);
		// FireBall->DamageParameters = GetDamageParameters();
		// FireBall->ExplosionDamageParams = GetDamageParameters();
		//
		// FireBall->SetOwner(GetAvatarActorFromActorInfo());
		//
		// FireBalls.Add(FireBall);
		// FireBall->FinishSpawning(SpawnTransform);
	}
	return FireBalls;
}
