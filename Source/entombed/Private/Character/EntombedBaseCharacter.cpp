// Copyright Yooshley


#include "Character/EntombedBaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/EntombedAbilitySystemComponent.h"


AEntombedBaseCharacter::AEntombedBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// setup item slots
	MainHandItem = CreateDefaultSubobject<USkeletalMeshComponent>("MainHandItem");
	MainHandItem->SetupAttachment(GetMesh(), FName("MainHandSocket"));
	MainHandItem->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OffHandItem = CreateDefaultSubobject<USkeletalMeshComponent>("OffHandItem");
	OffHandItem->SetupAttachment(GetMesh(), FName("OffHandSocket"));
	OffHandItem->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

void AEntombedBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEntombedBaseCharacter::InitializeAbilityActorInfo()
{
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

