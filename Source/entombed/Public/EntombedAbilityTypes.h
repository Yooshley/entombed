#pragma once

#include "GameplayEffectTypes.h"
#include "ScalableFloat.h"
#include "EntombedAbilityTypes.generated.h"

class AEntombedAbilityProjectile;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FEntombedProjectileData
{
	GENERATED_BODY();
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	bool bHasGravity = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile", meta = (EditCondition="bHasGravity", EditConditionHides))
	float Gravity = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile", meta = (EditCondition="bHasGravity", EditConditionHides))
	float Pitch = 0.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float Speed = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float Range = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float Scale = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	bool bHoming = false;

	UPROPERTY(EditDefaultsOnly, Category="Projectile", meta = (EditCondition="bHoming", EditConditionHides))
	float HomingAcceleration = 1500.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	bool bCanHitMultiple = false;
};

USTRUCT(BlueprintType)
struct FAbilityProjectileParameters
{
	GENERATED_BODY()
	FAbilityProjectileParameters(){}
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;
		
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	int32 Count = 1;
	
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	float Spread = 90.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AEntombedAbilityProjectile> ProjectileClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FEntombedProjectileData ProjectileData;
};

USTRUCT(BlueprintType)
struct FEntombedDebuffData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Debuff")
	FScalableFloat Chance;

	UPROPERTY(EditDefaultsOnly, Category="Debuff")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category="Debuff")
	FScalableFloat Duration;
	
	UPROPERTY(EditDefaultsOnly, Category="Debuff")
	FScalableFloat Period;
	
	UPROPERTY(EditDefaultsOnly, Category="Debuff|Stacking")
	bool bCanStack = false;

	UPROPERTY(EditDefaultsOnly, Category="Debuff|Stacking", meta = (EditCondition="bCanStack", EditConditionHides))
	int32 MaxStacks = 1;
};

USTRUCT(BlueprintType)
struct FEntombedDamageData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category="Damage")
    FGameplayTag Tag;

    UPROPERTY(EditDefaultsOnly, Category="Damage")
    FScalableFloat Value;
	
	UPROPERTY(EditDefaultsOnly, Category="Debuff")
	bool bCanDebuff = false;

	UPROPERTY(EditDefaultsOnly, Category="Debuff", meta=(EditCondition="bCanDebuff", EditConditionHides))
	FEntombedDebuffData Debuff;
	
	UPROPERTY(EditDefaultsOnly, Category="Knockback")
	bool bCanKnockback = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Knockback", meta=(EditCondition="bCanKnockback", EditConditionHides))
	float Force;
};

USTRUCT(BlueprintType)
struct FAppliedDebuffData
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag DebuffType;

	UPROPERTY()
	float Damage = 0.f;

	UPROPERTY()
	float Duration = 0.f;
	
	UPROPERTY()
	float Period = 0.f;
	
	UPROPERTY(BlueprintReadOnly)
	bool bCanStack = false;

	UPROPERTY(BlueprintReadOnly)
	int32 MaxStacks = 1;
	
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		DebuffType.NetSerialize(Ar, Map, bOutSuccess);
		Ar << Damage;
		Ar << Duration;
		Ar << Period;
		Ar << bCanStack;
        Ar << MaxStacks;
        
		bOutSuccess = true;
		return true;
	}
};

template<>
struct TStructOpsTypeTraits<FAppliedDebuffData> : public TStructOpsTypeTraitsBase2<FAppliedDebuffData>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};

USTRUCT(BlueprintType)
struct FAbilityDamageParameters
{
	GENERATED_BODY()
	FAbilityDamageParameters(){}

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;

	UPROPERTY()
	float AbilityLevel = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FEntombedDamageData> DamageTypes;
};

USTRUCT(BlueprintType)
struct FEntombedGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsCriticalHit() const { return bIsCriticalHit; }
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	
	bool HasDebuffs() const;
	bool HasDebuffType(const FGameplayTag& InDebuffType) const;
	const TArray<FAppliedDebuffData>& GetAppliedDebuffs() const;
	void AddDebuff(const FAppliedDebuffData& InDebuff);
	void ClearDebuffs();
	
	virtual	UScriptStruct*	GetScriptStruct() const
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FEntombedGameplayEffectContext* Duplicate() const
	{
		FEntombedGameplayEffectContext* NewContext = new FEntombedGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
	
protected:
	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	TArray<FAppliedDebuffData> AppliedDebuffs;
};

template<>
struct TStructOpsTypeTraits<FEntombedGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FEntombedGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};