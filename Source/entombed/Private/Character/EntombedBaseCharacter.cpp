// Copyright Yooshley


#include "Character/EntombedBaseCharacter.h"


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

