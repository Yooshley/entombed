// Copyright Yooshley


#include "AbilitySystem/Ability/EntombedProjectileAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/EntombedAbilitySystemLibrary.h"
#include "Actor/EntombedAbilityProjectile.h"
#include "Interface/CombatInterface.h"

UEntombedProjectileAbility::UEntombedProjectileAbility()
{

}

FAbilityDamageParameters UEntombedProjectileAbility::GetDamageParameters(AActor* TargetActor) const
{
	FAbilityDamageParameters DamageParameters;
	DamageParameters.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if(TargetActor) DamageParameters.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	DamageParameters.AbilityLevel = GetAbilityLevel();
	DamageParameters.DamageEffectClass = AbilityDamageParameters.DamageEffectClass;
	DamageParameters.DamageTypes = AbilityDamageParameters.DamageTypes;
	return DamageParameters;
}

FAbilityProjectileParameters UEntombedProjectileAbility::GetProjectileParameters(AActor* TargetActor) const
{
	FAbilityProjectileParameters ProjectileParameters;
	ProjectileParameters.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if(TargetActor) ProjectileParameters.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	ProjectileParameters.Count = GetAbilityLevel();
	ProjectileParameters.Spread = AbilityProjectileParameters.Spread;
	ProjectileParameters.ProjectileClass = AbilityProjectileParameters.ProjectileClass;
	ProjectileParameters.ProjectileData = AbilityProjectileParameters.ProjectileData;
	return ProjectileParameters;
}

void UEntombedProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo* ActorInfo, 
                                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                                 const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UEntombedProjectileAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEntombedProjectileAbility::SetTarget(FVector Location)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor) return;

	const FVector AvatarLocation = AvatarActor->GetActorLocation();

	TargetLocation = Location;
	const float MinZ = AvatarLocation.Z + 50.f; //ensure minimum height for projectile target
	
	TargetLocation.Z = FMath::Max(Location.Z, MinZ);

	if (AvatarActor->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_SetTargetDirection(AvatarActor, TargetLocation);
		ICombatInterface::Execute_SetOrientationMode(AvatarActor, true);
	}
}

void UEntombedProjectileAbility::SpawnProjectile(const FGameplayTag& SocketTag)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = (TargetLocation - SocketLocation).Rotation();
	
	const FVector Forward = Rotation.Vector();
	const int32 NumProjectiles = AbilityProjectileParameters.Count;
	TArray<FRotator> Rotations = UEntombedAbilitySystemLibrary::GetEvenlySpreadRotators(Forward, FVector::UpVector, AbilityProjectileParameters.Spread, NumProjectiles);
	
	for (const FRotator& Rotator : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotator.Quaternion());

		AEntombedAbilityProjectile* Projectile = GetWorld()->SpawnActorDeferred<AEntombedAbilityProjectile>(
		AbilityProjectileParameters.ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
		Projectile->DamageParameters = GetDamageParameters();
		Projectile->InitializeProjectile(AbilityProjectileParameters);
		Projectile->FinishSpawning(SpawnTransform);
	}
}
