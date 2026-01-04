// Copyright Yooshley


#include "Character/EntombedBaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "EntombedGameplayTags.h"
#include "AbilitySystem/EntombedAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "entombed/entombed.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Niagara/DebuffNiagaraComponent.h"
#include "Niagara/PassiveNiagaraComponent.h"
#include "UI/Widget/EntombedUserWidget.h"


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

	LifeBar = CreateDefaultSubobject<UWidgetComponent>("LifeBar");
	LifeBar->SetupAttachment(GetRootComponent());
	LifeBar->SetEnableGravity(false);

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

	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffComponent");
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = FEntombedGameplayTags::Get().Debuff_Burn;

	ShockDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("ShockDebuffComponent");
	ShockDebuffComponent->SetupAttachment(GetRootComponent());
	ShockDebuffComponent->DebuffTag = FEntombedGameplayTags::Get().Debuff_Shock;

	EffectAttachComponent = CreateDefaultSubobject<USceneComponent>("EffectAttachPoint");
	EffectAttachComponent->SetupAttachment(GetRootComponent());
	EffectAttachComponent->SetUsingAbsoluteRotation(true);

	PassiveDefenseComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("PassiveDefenseComponent");
	PassiveDefenseComponent->SetupAttachment(EffectAttachComponent);

	PassiveSiphonComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("PassiveSiphonComponent");
	PassiveSiphonComponent->SetupAttachment(EffectAttachComponent);
}

void AEntombedBaseCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEntombedBaseCharacter, bIsShocked);
	DOREPLIFETIME(AEntombedBaseCharacter, bIsBurned);
}

UAbilitySystemComponent* AEntombedBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float AEntombedBaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnDamage.Broadcast(DamageTaken);
	return DamageTaken;
}

void AEntombedBaseCharacter::OnRep_Shocked()
{
	
}

void AEntombedBaseCharacter::OnRep_Burned()
{
	
}

void AEntombedBaseCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsShocked = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsShocked ? 0.f : BaseRunSpeed;
}

FOnAbilitySystemReady& AEntombedBaseCharacter::GetOnAbilitySystemReadyDelegate()
{
	return OnAbilitySystemReady;
}

FOnDeathSignature& AEntombedBaseCharacter::GetOnDeathDelegate()
{
	return OnDeath;
}

UAnimMontage* AEntombedBaseCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AEntombedBaseCharacter::Death(const FVector& DeathImpulse)
{
	MainHandEquipment->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	OffHandEquipment->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath(DeathImpulse);
}

void AEntombedBaseCharacter::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	
	MainHandEquipment->SetSimulatePhysics(true);
	MainHandEquipment->SetEnableGravity(true);
	MainHandEquipment->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	MainHandEquipment->AddImpulse(DeathImpulse, NAME_None, true);

	OffHandEquipment->SetSimulatePhysics(true);
	OffHandEquipment->SetEnableGravity(true);
	OffHandEquipment->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	OffHandEquipment->AddImpulse(DeathImpulse, NAME_None, true);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	bDead = true;
	Dissolve();

	BurnDebuffComponent->Deactivate();
	ShockDebuffComponent->Deactivate();
	OnDeath.Broadcast(this);
}

void AEntombedBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UEntombedUserWidget* EntombedUserWidget = Cast<UEntombedUserWidget>(LifeBar->GetUserWidgetObject()))
	{
		EntombedUserWidget->SetWidgetController(this);
	}
}

void AEntombedBaseCharacter::InitializeAbilityActorInfo()
{
	if (const UEntombedAttributeSet* EntombedAS = CastChecked<UEntombedAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EntombedAS->GetLifeAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnLifeChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EntombedAS->GetTotalLifeAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnTotalLifeChanged.Broadcast(Data.NewValue);
			}
		);
    	
		OnLifeChanged.Broadcast(EntombedAS->GetLife());
		OnTotalLifeChanged.Broadcast(EntombedAS->GetTotalLife());
	}
}

int32 AEntombedBaseCharacter::GetCharacterLevel_Implementation()
{
	return Level;
}

void AEntombedBaseCharacter::SetCharacterLevel_Implementation(int32 InLevel)
{
	Level = InLevel;
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

void AEntombedBaseCharacter::SetCasting_Implementation(const bool bCast)
{
	bIsCasting = bCast;
}

bool AEntombedBaseCharacter::GetCasting_Implementation() const
{
	return bIsCasting;
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
		//bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
	else
	{
		//bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxWalkSpeed = BaseRunSpeed;
	}
}

int32 AEntombedBaseCharacter::GetMinionCount_Implementation()
{
	return MinionCount;
}

void AEntombedBaseCharacter::SetMinionCount_Implementation(int32 Count)
{
	MinionCount = Count;
}

EEntombedArchetype AEntombedBaseCharacter::GetArchetype_Implementation() const
{
	return Archetype;
}

void AEntombedBaseCharacter::SetArchetype_Implementation(EEntombedArchetype InArchetype)
{
	Archetype = InArchetype;
}

USkeletalMeshComponent* AEntombedBaseCharacter::GetMainHandEquipment_Implementation()
{
	return MainHandEquipment;
}

USkeletalMeshComponent* AEntombedBaseCharacter::GetOffHandEquipment_Implementation()
{
	return OffHandEquipment;
}

FOnDamageSignature& AEntombedBaseCharacter::GetOnDamageDelegate()
{
	return OnDamage;
}

void AEntombedBaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float InLevel=1.f) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, InLevel, ContextHandle);
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

