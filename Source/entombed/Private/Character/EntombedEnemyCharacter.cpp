// Copyright Yooshley


#include "Character/EntombedEnemyCharacter.h"

#include "EntombedGameplayTags.h"
#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "AbilitySystem/EntombedAbilitySystemLibrary.h"
#include "AbilitySystem/EntombedAttributeSet.h"
#include "AI/EntombedAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "entombed/entombed.h"
#include "GameFramework/CharacterMovementComponent.h"

AEntombedEnemyCharacter::AEntombedEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UEntombedAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UEntombedAttributeSet>("AttributeSet");
}

void AEntombedEnemyCharacter::PossessedBy(AController* NewController)
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

void AEntombedEnemyCharacter::HighlightActor()
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

void AEntombedEnemyCharacter::UnHighlightActor()
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

int32 AEntombedEnemyCharacter::GetCharacterLevel()
{
	return Level;
}

void AEntombedEnemyCharacter::Death()
{
	SetLifeSpan(LifeSpanPostDeath);
	if (EntombedAIController)
	{
		EntombedAIController->GetBlackboardComponent()->SetValueAsBool(BB_KEY_DEAD_BOOL, true);
	}
	Super::Death();
}

void AEntombedEnemyCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (EntombedAIController && EntombedAIController->GetBlackboardComponent())
	{
		EntombedAIController->GetBlackboardComponent()->SetValueAsBool(BB_KEY_HITREACTING_BOOL, bHitReacting);
	}
}

void AEntombedEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	
	InitializeAbilityActorInfo();

	if (HasAuthority())
	{
		UEntombedAbilitySystemLibrary::GiveDefaultAbilities(this, AbilitySystemComponent, Archetype);
	}

	if (const UEntombedAttributeSet* EntombedAS = CastChecked<UEntombedAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(FEntombedGameplayTags::Get().Effect_Knockback, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &AEntombedEnemyCharacter::HitReactTagChanged
		);
	}
}

void AEntombedEnemyCharacter::InitializeAbilityActorInfo()
{
	Super::InitializeAbilityActorInfo();
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UEntombedAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

void AEntombedEnemyCharacter::InitializeDefaultAttributes() const
{
	UEntombedAbilitySystemLibrary::InitializeDefaultAttributes(this, Archetype, Level, AbilitySystemComponent);
}
