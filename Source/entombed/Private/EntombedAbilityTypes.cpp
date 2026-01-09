#include "EntombedAbilityTypes.h"

bool FEntombedGameplayEffectContext::HasDebuffs() const
{
	return AppliedDebuffs.Num() > 0;
}

bool FEntombedGameplayEffectContext::HasDebuffType(const FGameplayTag& InDebuffType) const
{
	return AppliedDebuffs.ContainsByPredicate(
		[&InDebuffType](const FAppliedDebuffData& Debuff)
		{
			return Debuff.DebuffType == InDebuffType;
		});
}

const TArray<FAppliedDebuffData>& FEntombedGameplayEffectContext::GetAppliedDebuffs() const
{
	return AppliedDebuffs;
}

void FEntombedGameplayEffectContext::AddDebuff(const FAppliedDebuffData& InDebuff)
{
	AppliedDebuffs.Add(InDebuff);
}

void FEntombedGameplayEffectContext::ClearDebuffs()
{
	AppliedDebuffs.Reset();
}

bool FEntombedGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;

	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
			RepBits |= 1 << 0;
		if (bReplicateEffectCauser && EffectCauser.IsValid())
			RepBits |= 1 << 1;
		if (AbilityCDO.IsValid())
			RepBits |= 1 << 2;
		if (bReplicateSourceObject && SourceObject.IsValid())
			RepBits |= 1 << 3;
		if (Actors.Num() > 0)
			RepBits |= 1 << 4;
		if (HitResult.IsValid())
			RepBits |= 1 << 5;
		if (bHasWorldOrigin)
			RepBits |= 1 << 6;
		if (bIsCriticalHit)
			RepBits |= 1 << 7;
		if (AppliedDebuffs.Num() > 0)
			RepBits |= 1 << 8;
	}

	Ar.SerializeBits(&RepBits, 9);

	if (RepBits & (1 << 0)) Ar << Instigator;
	if (RepBits & (1 << 1)) Ar << EffectCauser;
	if (RepBits & (1 << 2)) Ar << AbilityCDO;
	if (RepBits & (1 << 3)) Ar << SourceObject;
	if (RepBits & (1 << 4)) SafeNetSerializeTArray_Default<31>(Ar, Actors);
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading() && !HitResult.IsValid())
		{
			HitResult = MakeShared<FHitResult>();
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}

	if (RepBits & (1 << 7))
	{
		Ar << bIsCriticalHit;
	}

	if (RepBits & (1 << 8))
	{
		int32 Num = AppliedDebuffs.Num();
		Ar << Num;

		if (Ar.IsLoading())
		{
			AppliedDebuffs.SetNum(Num);
		}

		for (int32 i = 0; i < Num; ++i)
		{
			AppliedDebuffs[i].NetSerialize(Ar, Map, bOutSuccess);
		}
	}
	else if (Ar.IsLoading())
	{
		AppliedDebuffs.Reset();
	}

	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get());
	}

	bOutSuccess = true;
	return true;
}
