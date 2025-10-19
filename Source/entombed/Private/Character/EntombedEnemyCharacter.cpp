// Copyright Yooshley


#include "Character/EntombedEnemyCharacter.h"

#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "AbilitySystem/EntombedAttributeSet.h"
#include "entombed/entombed.h"

AEntombedEnemyCharacter::AEntombedEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UEntombedAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UEntombedAttributeSet>("AttributeSet");
}

void AEntombedEnemyCharacter::HighlightActor()
{
	for (USkeletalMeshComponent* Part : EquipmentSlots)
	{
		if (!Part) continue;
		Part->SetRenderCustomDepth(true);
		Part->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
	MainHandItem->SetRenderCustomDepth(true);
	MainHandItem->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	OffHandItem->SetRenderCustomDepth(true);
	OffHandItem->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
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
	MainHandItem->SetRenderCustomDepth(false);
	OffHandItem->SetRenderCustomDepth(false);
	HeadAttachment->SetRenderCustomDepth(false);
}

void AEntombedEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilityActorInfo();
}

void AEntombedEnemyCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UEntombedAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
}
