// Copyright Yooshley


#include "Character/EntombedHostileCharacter.h"

#include "EntombedGameplayTags.h"
#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "AbilitySystem/EntombedAbilitySystemLibrary.h"
#include "AbilitySystem/EntombedAttributeSet.h"
#include "AI/EntombedAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "entombed/entombed.h"
#include "GameFramework/CharacterMovementComponent.h"

AEntombedHostileCharacter::AEntombedHostileCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UEntombedAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UEntombedAttributeSet>("AttributeSet");
}

void AEntombedHostileCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	
	EntombedAIController = Cast<AEntombedAIController>(NewController);
	EntombedAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	EntombedAIController->RunBehaviorTree(BehaviorTree);
	EntombedAIController->GetBlackboardComponent()->SetValueAsBool(BB_KEY_HITREACTING_BOOL, false);
	
	UArchetypeInfo* ArchetypeInfo = UEntombedAbilitySystemLibrary::GetArchetypeInfo(EntombedAIController);
	checkf(ArchetypeInfo, TEXT("Missing ArchetypeInfo for Archetype: [%s]"), *StaticEnum<EEntombedArchetype>()->GetValueAsString(Archetype));
	EntombedAIController->GetBlackboardComponent()->SetValueAsBool(BB_KEY_RANGED_BOOL, ArchetypeInfo->GetArchetypeDefaultInfo(Archetype).bIsRanged);
}

void AEntombedHostileCharacter::HighlightActor_Implementation()
{
	for (USkeletalMeshComponent* Part : EquipmentSlots)
	{
		if (!Part) continue;
		Part->SetRenderCustomDepth(true);
		Part->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
	MainHandEquipment->SetRenderCustomDepth(true);
	MainHandEquipment->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	OffHandEquipment->SetRenderCustomDepth(true);
	OffHandEquipment->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	HeadAttachment->SetRenderCustomDepth(true);
	HeadAttachment->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AEntombedHostileCharacter::UnHighlightActor_Implementation()
{
	for (USkeletalMeshComponent* Part : EquipmentSlots)
	{
		if (!Part) continue;
		Part->SetRenderCustomDepth(false);
	}
	MainHandEquipment->SetRenderCustomDepth(false);
	OffHandEquipment->SetRenderCustomDepth(false);
	HeadAttachment->SetRenderCustomDepth(false);
}

void AEntombedHostileCharacter::Death(const FVector& DeathImpulse)
{
	SetLifeSpan(LifeSpanPostDeath);
	if (EntombedAIController)
	{
		EntombedAIController->GetBlackboardComponent()->SetValueAsBool(BB_KEY_DEAD_BOOL, true);
	}
	SpawnLoot();
	Super::Death(DeathImpulse);
}

void AEntombedHostileCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseRunSpeed;
	if (EntombedAIController && EntombedAIController->GetBlackboardComponent())
	{
		EntombedAIController->GetBlackboardComponent()->SetValueAsBool(BB_KEY_HITREACTING_BOOL, bHitReacting);
	}
}

void AEntombedHostileCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseRunSpeed;

	if (HasAuthority())
	{
		UEntombedAbilitySystemLibrary::GrantDefaultAbilities(this, AbilitySystemComponent, Archetype);
	}

	if (const UEntombedAttributeSet* EntombedAS = CastChecked<UEntombedAttributeSet>(AttributeSet))
	{
		InitializeAbilityActorInfo();
		AbilitySystemComponent->RegisterGameplayTagEvent(FEntombedGameplayTags::Get().Effect_Knockback, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &AEntombedHostileCharacter::HitReactTagChanged
		);
	}
}

void AEntombedHostileCharacter::InitializeAbilityActorInfo()
{
	Super::InitializeAbilityActorInfo();
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UEntombedAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AbilitySystemComponent->RegisterGameplayTagEvent(FEntombedGameplayTags::Get().Debuff_Shock, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AEntombedHostileCharacter::StunTagChanged);

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}

	OnAbilitySystemReady.Broadcast(AbilitySystemComponent);
}

void AEntombedHostileCharacter::InitializeDefaultAttributes() const
{
	UEntombedAbilitySystemLibrary::InitializeDefaultAttributes(this, Archetype, Level, AbilitySystemComponent);
}

void AEntombedHostileCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);
	if (EntombedAIController && EntombedAIController->GetBlackboardComponent())
	{
		EntombedAIController->GetBlackboardComponent()->SetValueAsBool(BB_KEY_STUNNED_BOOL, bIsShocked);
	}
}
