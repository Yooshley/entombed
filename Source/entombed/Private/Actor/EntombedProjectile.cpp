// Copyright Yooshley


#include "Actor/EntombedProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/EntombedAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"
#include "entombed/entombed.h"

AEntombedProjectile::AEntombedProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->MaxSpeed = 1500.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AEntombedProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AEntombedProjectile::OnSphereOverlap);
	FlightAudioComponent = UGameplayStatics::SpawnSoundAttached(FlightSound, GetRootComponent());
}

void AEntombedProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation(), FRotator::ZeroRotator);
	if (FlightAudioComponent)
	{
		FlightAudioComponent->Stop();
		FlightAudioComponent->DestroyComponent();
	}
	bHit = true;
}

void AEntombedProjectile::Destroyed()
{
	if (FlightAudioComponent)
	{
		FlightAudioComponent->Stop();
		FlightAudioComponent->DestroyComponent();
	}
	if (!bHit && !HasAuthority())
	{
		OnHit();
	}
	Super::Destroyed();
}

bool AEntombedProjectile::IsValidOverlap(AActor* OtherActor)
{
	if (!DamageEffectParameters.SourceAbilitySystemComponent)
	{
		return false;
	}
	AActor* SourceAvatarActor = DamageEffectParameters.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor) return false;
	if (UEntombedAbilitySystemLibrary::IsAlly(SourceAvatarActor, OtherActor)) return false;
	return true;
}

void AEntombedProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	
	if(!bHit)
	{
		OnHit();
	}
	
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParameters.DeathImpulseMagnitude;
			DamageEffectParameters.DeathImpulse = DeathImpulse;
			
			const FVector KnockbackVector = GetActorForwardVector() * DamageEffectParameters.KnockbackMagnitude;
			DamageEffectParameters.KnockbackVector = KnockbackVector;
			
			DamageEffectParameters.TargetAbilitySystemComponent = TargetASC;
			UEntombedAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParameters);
		}
		
		Destroy();
	}
	else
	{
		bHit = true;
	}
}

