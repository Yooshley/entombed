// Copyright Yooshley


#include "Actor/EntombedAbilityProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/EntombedAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"
#include "entombed/entombed.h"

AEntombedAbilityProjectile::AEntombedAbilityProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	HitMesh = CreateDefaultSubobject<UStaticMeshComponent>("HitMesh");
	SetRootComponent(HitMesh);

	HitMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitMesh->SetCollisionObjectType(ECC_Projectile);
	HitMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	HitMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	HitMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HitMesh->SetGenerateOverlapEvents(true);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
}

void AEntombedAbilityProjectile::InitializeProjectile(const FAbilityProjectileParameters& InParameters)
{
	ProjectileParameters = InParameters;

	const FEntombedProjectileData& ProjectileData = ProjectileParameters.ProjectileData;

	if (ProjectileMovement)
	{
		ProjectileMovement->ProjectileGravityScale = ProjectileData.bHasGravity ? ProjectileData.Gravity : 0.f;
		ProjectileMovement->InitialSpeed = ProjectileData.Speed;
		ProjectileMovement->MaxSpeed = ProjectileData.Speed;
		
		SetLifeSpan(ProjectileData.Range/ProjectileData.Speed);

		if (ProjectileData.bHoming && ProjectileParameters.TargetAbilitySystemComponent)
		{
			AActor* TargetActor = ProjectileParameters.TargetAbilitySystemComponent->GetAvatarActor();
			if (TargetActor)
			{
				ProjectileMovement->HomingTargetComponent = TargetActor->GetRootComponent();
				ProjectileMovement->bIsHomingProjectile = true;
				ProjectileMovement->HomingAccelerationMagnitude = ProjectileData.HomingAcceleration;
			}
		}
		else
		{
			ProjectileMovement->bIsHomingProjectile = false;
		}
		
		bHitMultiple = ProjectileData.bCanHitMultiple;
	}
}

void AEntombedAbilityProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	
	if (HitMesh)
	{
		HitMesh->OnComponentBeginOverlap.AddDynamic(this, &AEntombedAbilityProjectile::OnHitMeshOverlap);
	}

	AudioComponent = UGameplayStatics::SpawnSoundAttached(TraversalSound, GetRootComponent());
}

void AEntombedAbilityProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation(), FRotator::ZeroRotator);
	
	if (!bHitMultiple)
	{
		if (AudioComponent)
		{
			AudioComponent->Stop();
			AudioComponent->DestroyComponent();
		}
		bHit = true;
	}
}

void AEntombedAbilityProjectile::Destroyed()
{
	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent->DestroyComponent();
	}
	if (!bHit && !HasAuthority())
	{
		OnHit();
	}
	Super::Destroyed();
}

bool AEntombedAbilityProjectile::IsValidOverlap(AActor* OtherActor) const
{
	if (!DamageParameters.SourceAbilitySystemComponent)
	{
		return false;
	}
	AActor* SourceAvatarActor = DamageParameters.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor) return false;
	if (UEntombedAbilitySystemLibrary::IsAlly(SourceAvatarActor, OtherActor)) return false;
	return true;
}

void AEntombedAbilityProjectile::OnHitMeshOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 BodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor))
		return;
	
	if (HitActors.Contains(OtherActor))
		return;
	
	HitActors.Add(OtherActor);

	if (!bHit)
	{
		OnHit();
	}

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			DamageParameters.TargetAbilitySystemComponent = TargetASC;
			UEntombedAbilitySystemLibrary::ApplyDamageEffect(DamageParameters);
		}

		if (!bHitMultiple) Destroy();
	}
	else
	{
		if (!bHitMultiple) bHit = true;
	}
}

