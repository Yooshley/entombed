// Copyright Yooshley


#include "Character/EntombedEnemyCharacter.h"

#include "EntombedGameplayTags.h"
#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "AbilitySystem/EntombedAbilitySystemLibrary.h"
#include "AbilitySystem/EntombedAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "entombed/entombed.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/EntombedUserWidget.h"

AEntombedEnemyCharacter::AEntombedEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UEntombedAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UEntombedAttributeSet>("AttributeSet");

	LifeBar = CreateDefaultSubobject<UWidgetComponent>("LifeBar");
	LifeBar->SetupAttachment(GetRootComponent());
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
	Super::Death();
}

void AEntombedEnemyCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

void AEntombedEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	
	InitializeAbilityActorInfo();

	if (HasAuthority())
	{
		UEntombedAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);
	}

	if (UEntombedUserWidget* EntombedUserWidget = Cast<UEntombedUserWidget>(LifeBar->GetUserWidgetObject()))
	{
		EntombedUserWidget->SetWidgetController(this);
	}

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
		
		AbilitySystemComponent->RegisterGameplayTagEvent(FEntombedGameplayTags::Get().Effect_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &AEntombedEnemyCharacter::HitReactTagChanged
		);
		
		OnLifeChanged.Broadcast(EntombedAS->GetLife());
		OnTotalLifeChanged.Broadcast(EntombedAS->GetTotalLife());
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
	UEntombedAbilitySystemLibrary::InitializeDefaultAttributes(this, Profession, Level, AbilitySystemComponent);
}
