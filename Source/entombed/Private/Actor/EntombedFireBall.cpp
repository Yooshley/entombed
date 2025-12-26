// Copyright Yooshley


#include "Actor/EntombedFireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EntombedGameplayTags.h"
#include "GameplayCueManager.h"
#include "AbilitySystem/EntombedAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"

void AEntombedFireBall::BeginPlay()
{
	Super::BeginPlay();

	StartOutgoingTimeline();
}

void AEntombedFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParameters.DeathImpulseMagnitude;
			DamageEffectParameters.DeathImpulse = DeathImpulse;
			
			DamageEffectParameters.TargetAbilitySystemComponent = TargetASC;
			UEntombedAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParameters);
		}
	}
}

void AEntombedFireBall::OnHit()
{
	if (GetOwner())
	{
		FGameplayCueParameters CueParameters;
		CueParameters.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FEntombedGameplayTags::Get().GameplayCue_FireBlast, CueParameters);
	}
	
	if (FlightAudioComponent)
	{
		FlightAudioComponent->Stop();
		FlightAudioComponent->DestroyComponent();
	}
	bHit = true;
}
