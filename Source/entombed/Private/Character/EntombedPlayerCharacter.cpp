// Copyright Yooshley


#include "Character/EntombedPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "EntombedGameplayTags.h"
#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "AbilitySystem/EntombedAbilitySystemLibrary.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "Player/EntombedPlayerController.h"
#include "Player/EntombedPlayerState.h"
#include "UI/HUD/EntombedHUD.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/EntombedAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Camera/CameraComponent.h"
#include "Game/EntombedGameModeBase.h"
#include "Game/EntombedSaveGame.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Niagara/DebuffNiagaraComponent.h"

AEntombedPlayerCharacter::AEntombedPlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
	
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
}

void AEntombedPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AEntombedPlayerState* PS = GetPlayerState<AEntombedPlayerState>();
	check(PS);

	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	ASC->InitAbilityActorInfo(PS, this);

	if (UEntombedAbilitySystemComponent* EntombedASC = Cast<UEntombedAbilitySystemComponent>(ASC))
	{
		EntombedASC->AbilityActorInfoSet();
	}

	AbilitySystemComponent = ASC;
	AttributeSet = PS->GetAttributeSet();
	
	InitializeDefaultAttributes();

	if (HasAuthority())
	{
		UEntombedAbilitySystemLibrary::GrantDefaultAbilities(this, AbilitySystemComponent, Archetype);
	}
	
	InitializeAbilityActorInfo();
}

void AEntombedPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AEntombedPlayerState* PS = GetPlayerState<AEntombedPlayerState>();
	check(PS);

	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	ASC->InitAbilityActorInfo(PS, this);

	if (UEntombedAbilitySystemComponent* EntombedASC = Cast<UEntombedAbilitySystemComponent>(ASC))
	{
		EntombedASC->AbilityActorInfoSet();
	}

	AbilitySystemComponent = ASC;
	AttributeSet = PS->GetAttributeSet();
	
	if (IsLocallyControlled())
	{
		if (AEntombedPlayerController* PC = Cast<AEntombedPlayerController>(GetController()))
		{
			if (AEntombedHUD* HUD = Cast<AEntombedHUD>(PC->GetHUD()))
			{
				HUD->InitializeOverlay(PC, PS, AbilitySystemComponent, AttributeSet);
			}
		}
	}
	
	InitializeAbilityActorInfo();
}

void AEntombedPlayerCharacter::AddXP_Implementation(int32 InXP)
{
	AEntombedPlayerState* EntombedPlayerState = GetPlayerState<AEntombedPlayerState>();
	check(EntombedPlayerState);
	EntombedPlayerState->AddXP(InXP);
}

void AEntombedPlayerCharacter::AddLevel_Implementation(int32 InLevel)
{
	AEntombedPlayerState* EntombedPlayerState = GetPlayerState<AEntombedPlayerState>();
	check(EntombedPlayerState);
	EntombedPlayerState->AddLevel(InLevel);
	
	if (UEntombedAbilitySystemComponent* EntombedASC = Cast<UEntombedAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		EntombedASC->UpdateAbilityStatus(EntombedPlayerState->GetPlayerLevel());
	}
}

void AEntombedPlayerCharacter::AddAttributePoints_Implementation(int32 InAttributePoints)
{
	AEntombedPlayerState* EntombedPlayerState = GetPlayerState<AEntombedPlayerState>();
	check(EntombedPlayerState);
	EntombedPlayerState->AddAttributePoints(InAttributePoints);
}

void AEntombedPlayerCharacter::AddAbilityPoints_Implementation(int32 InAbilityPoints)
{
	AEntombedPlayerState* EntombedPlayerState = GetPlayerState<AEntombedPlayerState>();
    check(EntombedPlayerState);
    EntombedPlayerState->AddAbilityPoints(InAbilityPoints);
}

int32 AEntombedPlayerCharacter::GetXP_Implementation() const
{
	AEntombedPlayerState* EntombedPlayerState = GetPlayerState<AEntombedPlayerState>();
	check(EntombedPlayerState);
	return EntombedPlayerState->GetXP();
}

int32 AEntombedPlayerCharacter::GetAttributePointsAward_Implementation(int32 InLevel) const
{
	AEntombedPlayerState* EntombedPlayerState = GetPlayerState<AEntombedPlayerState>();
	check(EntombedPlayerState);
	return EntombedPlayerState->LevelInfo->LevelInformation[InLevel].AttributeAward;
}

int32 AEntombedPlayerCharacter::GetAbilityPointsAward_Implementation(int32 InLevel) const
{
	AEntombedPlayerState* EntombedPlayerState = GetPlayerState<AEntombedPlayerState>();
	check(EntombedPlayerState);
	return EntombedPlayerState->LevelInfo->LevelInformation[InLevel].AbilityAward;
}

int32 AEntombedPlayerCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	AEntombedPlayerState* EntombedPlayerState = GetPlayerState<AEntombedPlayerState>();
	check(EntombedPlayerState);
	return EntombedPlayerState->LevelInfo->FindLevelForXP(InXP);
}

void AEntombedPlayerCharacter::LevelUp_Implementation()
{
	MulticastLevelUpVFX();
}

int32 AEntombedPlayerCharacter::GetAttributePoints_Implementation() const
{
	AEntombedPlayerState* EntombedPlayerState = GetPlayerState<AEntombedPlayerState>();
	check(EntombedPlayerState);
	return EntombedPlayerState->GetAttributePoints();
}

int32 AEntombedPlayerCharacter::GetAbilityPoints_Implementation() const
{
	AEntombedPlayerState* EntombedPlayerState = GetPlayerState<AEntombedPlayerState>();
	check(EntombedPlayerState);
	return EntombedPlayerState->GetAbilityPoints();
}

void AEntombedPlayerCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (AEntombedPlayerController* EntombedPlayerController = Cast<AEntombedPlayerController>(GetController()))
	{
		EntombedPlayerController->ShowMagicCircle(DecalMaterial);
	}
}

void AEntombedPlayerCharacter::HideMagicCircle_Implementation()
{
	if (AEntombedPlayerController* EntombedPlayerController = Cast<AEntombedPlayerController>(GetController()))
	{
		EntombedPlayerController->HideMagicCircle();
	}
}

void AEntombedPlayerCharacter::SavePlayerProgress_Implementation(const FName& CheckpointTag)
{
	AEntombedGameModeBase* EntombedGameMode = Cast<AEntombedGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (EntombedGameMode)
	{
		UEntombedSaveGame* SaveData = EntombedGameMode->GetGameSaveData();
		if (SaveData == nullptr) return;
		
		SaveData->CheckpointTag = CheckpointTag;
		
		if (AEntombedPlayerState* EntombedPlayerState = Cast<AEntombedPlayerState>(GetPlayerState()))
		{
			SaveData->PlayerLevel = EntombedPlayerState->GetPlayerLevel();
			SaveData->XP = EntombedPlayerState->GetXP();
			SaveData->AttributePoints = EntombedPlayerState->GetAttributePoints();
			SaveData->AbilityPoints = EntombedPlayerState->GetAbilityPoints();
		}
		
		SaveData->bFreshSave = false;
		
		if(!HasAuthority()) return;
		
		UEntombedAbilitySystemComponent* EntombedASC = Cast<UEntombedAbilitySystemComponent>(AbilitySystemComponent);
		
		FForEachAbility SaveAbilityDelegate;
		SaveData->SavedAbilities.Empty();
		SaveAbilityDelegate.BindLambda([this, EntombedASC, SaveData](const FGameplayAbilitySpec& AbilitySpec)
		{
			const FGameplayTag AbilityTag = EntombedASC->GetAbilityTagFromSpec(AbilitySpec);
			UAbilityInfo* AbilityInfo = UEntombedAbilitySystemLibrary::GetAbilityInfo(this);
			FEntombedAbilityInfo EntombedAbilityInfo = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
			
			FSavedAbility SavedAbility;
			SavedAbility.GameplayAbility = EntombedAbilityInfo.AbilityClass;
			SavedAbility.AbilityLevel = AbilitySpec.Level;
			SavedAbility.AbilitySlot = EntombedASC->GetSlotTagFromAbilityTag(AbilityTag);
			SavedAbility.AbilityStatus = EntombedASC->GetStatusTagFromAbilityTag(AbilityTag);
			SavedAbility.AbilityType = EntombedAbilityInfo.TypeTag;
			SavedAbility.AbilityTag = AbilityTag;
			
			SaveData->SavedAbilities.AddUnique(SavedAbility);
		});
		EntombedASC->ForEachAbility(SaveAbilityDelegate);
		
		EntombedGameMode->SaveGameData(SaveData);
	}
}

int32 AEntombedPlayerCharacter::GetCharacterLevel_Implementation()
{
	const AEntombedPlayerState* EntombedPlayerState = GetPlayerState<AEntombedPlayerState>();
	check(EntombedPlayerState);
	
	return EntombedPlayerState->GetPlayerLevel();
}

void AEntombedPlayerCharacter::Death(const FVector& DeathImpulse)
{
	Super::Death(DeathImpulse);
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this]()
	{
		AEntombedGameModeBase* EntombedGameMode = Cast<AEntombedGameModeBase>(UGameplayStatics::GetGameMode(this));
		if (EntombedGameMode)
		{
			EntombedGameMode->HandlePlayerDeath(this);
		}
	});
	GetWorldTimerManager().SetTimer(DeathTimer, TimerDelegate, DeathTime, false);
	CameraComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void AEntombedPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	//if (HasAuthority()) InitializeAbilityActorInfo();
}

void AEntombedPlayerCharacter::LoadPlayerProgress()
{
	AEntombedGameModeBase* EntombedGameMode = Cast<AEntombedGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (EntombedGameMode)
	{
		UEntombedSaveGame* SaveData = EntombedGameMode->GetGameSaveData();
		if (SaveData == nullptr) return;
		
		if (SaveData->bFreshSave)
		{
			InitializeDefaultAttributes();
		}
		else
		{
			if (UEntombedAbilitySystemComponent* EntombedASC = Cast<UEntombedAbilitySystemComponent>(AbilitySystemComponent))
			{
				EntombedASC->GrantAbilitiesFromSaveData(SaveData);
			}
			
			if (AEntombedPlayerState* EntombedPlayerState = Cast<AEntombedPlayerState>(GetPlayerState()))
			{
				EntombedPlayerState->SetLevel(SaveData->PlayerLevel);
				EntombedPlayerState->SetXP(SaveData->XP);
				EntombedPlayerState->SetAttributePoints(SaveData->AttributePoints);
				EntombedPlayerState->SetAbilityPoints(SaveData->AbilityPoints);
			}
			UEntombedAbilitySystemLibrary::InitializeAttributesFromSaveData(this, GetAbilitySystemComponent(), SaveData); 
		}
	}
}

void AEntombedPlayerCharacter::InitializeAbilityActorInfo()
{
    Super::InitializeAbilityActorInfo();
	
	const UEntombedAttributeSet* AS = CastChecked<UEntombedAttributeSet>(AttributeSet);

	OnLifeChanged.Broadcast(AS->GetLife());
	OnTotalLifeChanged.Broadcast(AS->GetTotalLife());
	OnFormChanged.Broadcast(AS->GetForm());
	OnTotalFormChanged.Broadcast(AS->GetTotalForm());

	OnAbilitySystemReady.Broadcast(AbilitySystemComponent);
}

void AEntombedPlayerCharacter::OnRep_Shocked()
{
	if (UEntombedAbilitySystemComponent* EntombedASC = Cast<UEntombedAbilitySystemComponent>(AbilitySystemComponent))
	{
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(FEntombedGameplayTags::Get().Player_Block_InputPressed);
		BlockedTags.AddTag(FEntombedGameplayTags::Get().Player_Block_InputReleased);
		BlockedTags.AddTag(FEntombedGameplayTags::Get().Player_Block_InputHeld);

		if (bIsShocked)
		{
			EntombedASC->AddLooseGameplayTags(BlockedTags);
			ShockDebuffComponent->Activate();
		}
		else
		{
			EntombedASC->RemoveLooseGameplayTags(BlockedTags);
			ShockDebuffComponent->Deactivate();
		}
	}
}

void AEntombedPlayerCharacter::OnRep_Burned()
{
	if (bIsBurned)
	{
		BurnDebuffComponent->Activate();
	}
	else
	{
		BurnDebuffComponent->Deactivate();
	}
}

void AEntombedPlayerCharacter::MulticastLevelUpVFX_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = CameraComponent->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator FaceCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(FaceCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}
