// Copyright Yooshley


#include "AbilitySystem/Ability/EntombedBeamAbility.h"

#include "AbilitySystem/EntombedAbilitySystemLibrary.h"
#include "entombed/entombed.h"
#include "GameFramework/Character.h"
#include "entombed/Public/Interface/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UEntombedBeamAbility::StoreCursorDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		CursorHitLocation = HitResult.Location;
		CursorHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UEntombedBeamAbility::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UEntombedBeamAbility::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if(OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* Equipment = ICombatInterface::Execute_GetMainHandEquipment(OwnerCharacter))
		{
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(OwnerCharacter);
			FHitResult HitResult;
			const FVector SocketLocation = Equipment->GetSocketLocation(TIP_SOCKET_NAME);
			UKismetSystemLibrary::SphereTraceSingle(
				OwnerCharacter,
				SocketLocation,
				BeamTargetLocation,
				10.f,
				TraceTypeQuery1,
				false, ActorsToIgnore,
				EDrawDebugTrace::None,
				HitResult,
				true);

			if (HitResult.bBlockingHit)
			{
				CursorHitLocation = HitResult.Location;
				CursorHitActor = HitResult.GetActor();
			}
		}
	}
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(CursorHitActor))
	{
		if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UEntombedBeamAbility::PrimaryTargetDead))
		{
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UEntombedBeamAbility::PrimaryTargetDead);
		}
	}
}

void UEntombedBeamAbility::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(CursorHitActor);
	
	TArray<AActor*> OverlappingActors;
	UEntombedAbilitySystemLibrary::GetLiveActorsWithinRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		ActorsToIgnore,
		750.f, //TODO: Parameterize
		CursorHitActor->GetActorLocation());

	int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1, MaxChainTargets);
	//int32 NumAdditionalTargets = MaxChainTargets; //for testing

	UEntombedAbilitySystemLibrary::GetClosestActors(NumAdditionalTargets, OverlappingActors, OutAdditionalTargets, CursorHitActor->GetActorLocation());

	for (AActor* AdditionalTarget : OutAdditionalTargets)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(AdditionalTarget))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UEntombedBeamAbility::AdditionalTargetDead))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UEntombedBeamAbility::AdditionalTargetDead);
			}
		}
	}
}
