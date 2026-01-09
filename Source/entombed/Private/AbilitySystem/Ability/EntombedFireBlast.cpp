// Copyright Yooshley


#include "AbilitySystem/Ability/EntombedFireBlast.h"

#include "AbilitySystem/EntombedAbilitySystemLibrary.h"
#include "Actor/EntombedFireBall.h"

FString UEntombedFireBlast::GetLevelDescription(int32 Level)
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

FString UEntombedFireBlast::GetNextLevelDescription(int32 Level)
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

TArray<AEntombedFireBall*> UEntombedFireBlast::SpawnFireBalls()
{
	TArray<AEntombedFireBall*> FireBalls;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UEntombedAbilitySystemLibrary::GetEvenlySpreadRotators(Forward, FVector::UpVector, 360.f, NumFireBalls);

	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		AEntombedFireBall* FireBall = GetWorld()->SpawnActorDeferred<AEntombedFireBall>(
			FireBallClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);
		FireBall->DamageEffectParameters = GetDefaultDamageParameters();
		FireBall->ExplosionDamageParams = GetDefaultDamageParameters();
		
		FireBall->SetOwner(GetAvatarActorFromActorInfo());
		
		FireBalls.Add(FireBall);
		FireBall->FinishSpawning(SpawnTransform);
	}
	return FireBalls;
}
