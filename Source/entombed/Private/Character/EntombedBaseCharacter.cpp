// Copyright Yooshley


#include "Character/EntombedBaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "entombed/entombed.h"


AEntombedBaseCharacter::AEntombedBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	// setup item slots
	MainHandEquipment = CreateDefaultSubobject<USkeletalMeshComponent>("MainHandEquipment");
	MainHandEquipment->SetupAttachment(GetMesh(), FName("MainHandSocket"));
	MainHandEquipment->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OffHandEquipment = CreateDefaultSubobject<USkeletalMeshComponent>("OffHandEquipment");
	OffHandEquipment->SetupAttachment(GetMesh(), FName("OffHandSocket"));
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
	HeadAttachment->SetupAttachment(GetMesh(), FName("HeadSocket"));
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
	MulticastHandleDeath_Implementation();
}

void AEntombedBaseCharacter::MulticastHandleDeath_Implementation()
{
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
	Dissolve();
}

void AEntombedBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEntombedBaseCharacter::InitializeAbilityActorInfo()
{
}

FVector AEntombedBaseCharacter::GetMainHandSocketLocation()
{
	check(MainHandEquipment);
	return MainHandEquipment->GetSocketLocation(FName(MainHandTipSocketName));
}

FVector AEntombedBaseCharacter::GetOffHandSocketLocation()
{
	check(OffHandEquipment);
	return OffHandEquipment->GetSocketLocation(FName(OffHandTipSocketName));
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

void AEntombedBaseCharacter::AddDefaultAbilities()
{
	UEntombedAbilitySystemComponent* ASC = CastChecked<UEntombedAbilitySystemComponent>(GetAbilitySystemComponent());
	if (!HasAuthority()) return;

	ASC->AddDefaultAbilities(DefaultAbilities);
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

