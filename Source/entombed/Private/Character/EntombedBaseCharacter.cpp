// Copyright Yooshley


#include "Character/EntombedBaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "EntombedGameplayTags.h"
#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "entombed/entombed.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/EntombedPlayerController.h"


class AEntombedPlayerController;

AEntombedBaseCharacter::AEntombedBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->MaxWalkSpeed = BaseRunSpeed;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	// setup item slots
	MainHandEquipment = CreateDefaultSubobject<USkeletalMeshComponent>("MainHandEquipment");
	MainHandEquipment->SetupAttachment(GetMesh(), MAIN_HAND_SOCKET_NAME);
	MainHandEquipment->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OffHandEquipment = CreateDefaultSubobject<USkeletalMeshComponent>("OffHandEquipment");
	OffHandEquipment->SetupAttachment(GetMesh(), OFF_HAND_SOCKET_NAME);
	OffHandEquipment->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// setup equipment slots
	HeadEquipment     = CreateEquipmentSlot(TEXT("HeadEquipment"));
	TorsoEquipment    = CreateEquipmentSlot(TEXT("TorsoEquipment"));
	ArmLeftEquipment  = CreateEquipmentSlot(TEXT("ArmLeftEquipment"));
	ArmRightEquipment = CreateEquipmentSlot(TEXT("ArmRightEquipment"));
	LegLeftEquipment  = CreateEquipmentSlot(TEXT("LegLeftEquipment"));
	LegRightEquipment = CreateEquipmentSlot(TEXT("LegRightEquipment"));

	EquipmentSlots = { HeadEquipment, TorsoEquipment, ArmLeftEquipment, ArmRightEquipment, LegLeftEquipment, LegRightEquipment };
	SetupDefaultEquipment();

	// setup attachment slots
	HeadAttachment = CreateDefaultSubobject<USkeletalMeshComponent>("HeadAttachment");
	HeadAttachment->SetupAttachment(GetMesh(), HEAD_SOCKET_NAME);
	HeadAttachment->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AEntombedBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AEntombedBaseCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AEntombedBaseCharacter::Death()
{
	MainHandEquipment->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	OffHandEquipment->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

void AEntombedBaseCharacter::MulticastHandleDeath_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	
	MainHandEquipment->SetSimulatePhysics(true);
	MainHandEquipment->SetEnableGravity(true);
	MainHandEquipment->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	OffHandEquipment->SetSimulatePhysics(true);
	OffHandEquipment->SetEnableGravity(true);
	OffHandEquipment->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	bDead = true;
	Dissolve();
}

void AEntombedBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEntombedBaseCharacter::InitializeAbilityActorInfo()
{
}

FVector AEntombedBaseCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FEntombedGameplayTags& GameplayTags = FEntombedGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.Socket_MainHand))
	{
		if (IsValid(MainHandEquipment))
		{
			return MainHandEquipment->GetSocketLocation(TIP_SOCKET_NAME);
		}
		return GetMesh()->GetSocketLocation(MAIN_HAND_SOCKET_NAME); //unarmed mainhand
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Socket_OffHand))
	{
		if (IsValid(OffHandEquipment))
		{
			return OffHandEquipment->GetSocketLocation(TIP_SOCKET_NAME);
		}
		return GetMesh()->GetSocketLocation(FName(OFF_HAND_SOCKET_NAME)); //unarmed offhand
	}
	return FVector();
}

bool AEntombedBaseCharacter::IsDead_Implementation() const
{
	return bDead;
}

AActor* AEntombedBaseCharacter::GetAvatarActor_Implementation()
{
	return this;
}

TArray<FTaggedMontage> AEntombedBaseCharacter::GetTaggedMontages_Implementation()
{
	return TaggedMontages;
}

UNiagaraSystem* AEntombedBaseCharacter::GetImpactEffect_Implementation()
{
	return ImpactEffect;
}

FTaggedMontage AEntombedBaseCharacter::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for(FTaggedMontage& TaggedMontage : TaggedMontages)
	{
		if (TaggedMontage.MontageTag.MatchesTagExact(MontageTag))
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

void AEntombedBaseCharacter::SetTarget_Implementation(AActor* InTarget)
{
	TargetActor = InTarget;
}

AActor* AEntombedBaseCharacter::GetTarget_Implementation() const
{
	return TargetActor;
}

FRotator AEntombedBaseCharacter::GetTargetDirection_Implementation() const
{
	return TargetDirection;
}

void AEntombedBaseCharacter::SetTargetDirection_Implementation(FVector TargetLocation)
{
	FVector TargetVector = TargetLocation - GetActorLocation();
	TargetVector.Z = 0.f;

	if (!TargetVector.IsNearlyZero())
	{
		TargetDirection = TargetVector.Rotation();
	}
}

void AEntombedBaseCharacter::SetOrientationMode_Implementation(const bool bStrafe)
{
	if (bStrafe)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxWalkSpeed = BaseRunSpeed;
	}
}

void AEntombedBaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level=1.f) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AEntombedBaseCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultCoreAttributes);
	ApplyEffectToSelf(DefaultDerivedAttributes);
	ApplyEffectToSelf(DefaultResourceAttributes);
}

void AEntombedBaseCharacter::Dissolve()
{
	if (!IsValid(DissolveMaterialInstance)) return;

	//TODO: nested for loop + dynamic instance for each equipment slot for each dead character might not be performance friendly->figure out better alternative
	TArray<UMaterialInstanceDynamic*> DynamicMaterialInstances;
    for (const TObjectPtr<USkeletalMeshComponent> Equipment : EquipmentSlots)
    {
    	if (!IsValid(Equipment)) continue;
    	
    	const int32 MaterialCount = Equipment->GetNumMaterials();
    	if (MaterialCount <= 0) continue;

    	UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
    	for (int32 i = 0; i < MaterialCount; ++i)
    	{
    		Equipment->SetMaterial(i, DynamicMatInst);
    	}
    	DynamicMaterialInstances.AddUnique(DynamicMatInst);
    }
	if (IsValid(MainHandEquipment))
	{
		const int32 MaterialCount = MainHandEquipment->GetNumMaterials();
		if (MaterialCount > 0)
		{
			UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
			for (int32 i = 0; i < MaterialCount; ++i)
			{
				MainHandEquipment->SetMaterial(i, DynamicMatInst);
			}
			DynamicMaterialInstances.AddUnique(DynamicMatInst);
		}
	}
	if (IsValid(OffHandEquipment))
	{
		const int32 MaterialCount = OffHandEquipment->GetNumMaterials();
		if (MaterialCount > 0)
		{
			UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
			for (int32 i = 0; i < MaterialCount; ++i)
			{
				OffHandEquipment->SetMaterial(i, DynamicMatInst);
			}
			DynamicMaterialInstances.AddUnique(DynamicMatInst);
		}
	}
	if (IsValid(HeadAttachment))
	{
		const int32 MaterialCount = HeadAttachment->GetNumMaterials();
		if (MaterialCount > 0)
		{
			UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
			for (int32 i = 0; i < MaterialCount; ++i)
			{
				HeadAttachment->SetMaterial(i, DynamicMatInst);
			}
			DynamicMaterialInstances.AddUnique(DynamicMatInst);
		}
	}
    StartDissolveTimeline(DynamicMaterialInstances);
}

USkeletalMeshComponent* AEntombedBaseCharacter::CreateEquipmentSlot(const FName& Name)
{
	USkeletalMeshComponent* Part = CreateDefaultSubobject<USkeletalMeshComponent>(Name);
	Part->SetupAttachment(GetMesh());
	Part->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Part->bUseAttachParentBound = true;
	Part->SetGenerateOverlapEvents(false);
	Part->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	return Part;
}

void AEntombedBaseCharacter::SetupDefaultEquipment()
{
	USkeletalMeshComponent* Leader = GetMesh();
	if (!Leader) return;

	for (USkeletalMeshComponent* Part : EquipmentSlots)
	{
		if (!Part) continue;
		Part->SetLeaderPoseComponent(Leader);
		Part->bUseAttachParentBound = true;
	}
}

