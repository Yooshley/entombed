// Copyright Yooshley


#include "AbilitySystem/Ability/UDEPRECATED_UEntombedRangedAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EntombedGameplayTags.h"
#include "Actor/EntombedAbilityProjectile.h"
#include "entombed/Public/Interface/CombatInterface.h"

void UDEPRECATED_UEntombedRangedAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UDEPRECATED_UEntombedRangedAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDEPRECATED_UEntombedRangedAbility::SetProjectileTarget(FVector Location)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor) return;

	const FVector AvatarLocation = AvatarActor->GetActorLocation();

	ProjectileTargetLocation = Location;
	const float MinZ = AvatarLocation.Z + 50.f; //ensure minimum height for projectile target
	
	ProjectileTargetLocation.Z = FMath::Max(Location.Z, MinZ);

	if (AvatarActor->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_SetTargetDirection(AvatarActor, ProjectileTargetLocation);
		ICombatInterface::Execute_SetOrientationMode(AvatarActor, true);
	}
}

void UDEPRECATED_UEntombedRangedAbility::SpawnProjectile(const FGameplayTag& SocketTag)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
	
	AEntombedAbilityProjectile* Projectile = GetWorld()->SpawnActorDeferred<AEntombedAbilityProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	
	Projectile->DamageParameters = GetDamageParameters();
	Projectile->FinishSpawning(SpawnTransform);
}
