// Copyright Yooshley


#include "AbilitySystem/Ability/EntombedFireBolt.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/EntombedAbilitySystemLibrary.h"
#include "Actor/EntombedAbilityActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "entombed/Public/Interface/CombatInterface.h"

FString UEntombedFireBolt::GetLevelDescription(int32 Level)
{
	const int32 ScaledDamage = 0;
	const float Cost = FMath::Abs(GetCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		"<Title>FIRE BOLT</>\n\n"

		"<Small>Level: </><Level>%d</>\n"
		"<Small>Cost: </><Cost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			
		"<Default>Launches %d bolt of fire, exploding on impact and dealing </>"
		"<Damage>%d Damage.</>"), Level, Cost, Cooldown, Level, ScaledDamage);
}

FString UEntombedFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = 0;
	const float Cost = FMath::Abs(GetCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL</>\n\n"

		"<Small>Level: </><Level>%d</>\n"
		"<Small>Cost: </><Cost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
			
		"<Default>Launches %d bolt of fire, exploding on impact and dealing </>"
		"<Damage>%d Damage.</>"), Level, Cost, Cooldown, Level, ScaledDamage);
}

void UEntombedFireBolt::SpawnProjectiles(const FGameplayTag& SocketTag, AActor* HomingTarget, bool bPitchOverride, float PitchOverride)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bPitchOverride)
	{
		Rotation.Pitch = PitchOverride;
	}

	const FVector Forward = Rotation.Vector();
	const int32 NumProjectiles = FMath::Min(ProjectileCount, GetAbilityLevel());
	//const int32 NumProjectiles = ProjectileCount; //for testing

	TArray<FRotator> Rotations = UEntombedAbilitySystemLibrary::GetEvenlySpreadRotators(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);

	for (const FRotator& Rotator : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotator.Quaternion());

		AEntombedAbilityActor* Projectile = GetWorld()->SpawnActorDeferred<AEntombedAbilityActor>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
		Projectile->DamageEffectParameters = GetDefaultDamageParameters();

		Projectile->ProjectileMovement->bIsHomingProjectile = bHomingProjectile;
		if (bHomingProjectile)
		{
			if(HomingTarget && HomingTarget->Implements<UCombatInterface>())
			{
				Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
			}
			else
			{
				Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
				Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
				Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
			}
			Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		}
		Projectile->FinishSpawning(SpawnTransform);
	}
}
