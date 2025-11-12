// Copyright Yooshley


#include "AbilitySystem/Ability/EntombedSummonAbility.h"

TArray<FVector> UEntombedSummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread/2.f, FVector::UpVector);
	TArray<FVector> SpawnLocations;
	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread*i, FVector::UpVector);
		FVector SpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, SpawnLocation + FVector(0.f, 0.f, 500.f), SpawnLocation - FVector(0.f, 0.f, 500.f), ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			SpawnLocation = Hit.ImpactPoint;
		}
		SpawnLocations.Add(SpawnLocation);
	}
	return SpawnLocations;
}

TSubclassOf<APawn> UEntombedSummonAbility::GetRandomMinionClass()
{
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection];
}
