// Copyright Yooshley


#include "Character/EntombedPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "AbilitySystem/EntombedAbilitySystemLibrary.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "Player/EntombedPlayerController.h"
#include "Player/EntombedPlayerState.h"
#include "UI/HUD/EntombedHUD.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

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

	// init ability actor info for the server
	InitializeAbilityActorInfo();
}

void AEntombedPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// init ability actor info for the client
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

int32 AEntombedPlayerCharacter::GetAttributePointsAward_Implementation(int32 Level) const
{
	AEntombedPlayerState* EntombedPlayerState = GetPlayerState<AEntombedPlayerState>();
	check(EntombedPlayerState);
	return EntombedPlayerState->LevelInfo->LevelInformation[Level].AttributeAward;
}

int32 AEntombedPlayerCharacter::GetAbilityPointsAward_Implementation(int32 Level) const
{
	AEntombedPlayerState* EntombedPlayerState = GetPlayerState<AEntombedPlayerState>();
	check(EntombedPlayerState);
	return EntombedPlayerState->LevelInfo->LevelInformation[Level].AbilityAward;
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

int32 AEntombedPlayerCharacter::GetCharacterLevel_Implementation()
{
	const AEntombedPlayerState* EntombedPlayerState = GetPlayerState<AEntombedPlayerState>();
	check(EntombedPlayerState);
	
	return EntombedPlayerState->GetPlayerLevel();
}

void AEntombedPlayerCharacter::InitializeAbilityActorInfo()
{
	AEntombedPlayerState* EntombedPlayerState = GetPlayerState<AEntombedPlayerState>();
	check(EntombedPlayerState);
	
	EntombedPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(EntombedPlayerState, this);
	Cast<UEntombedAbilitySystemComponent>(EntombedPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = EntombedPlayerState->GetAbilitySystemComponent();
	AttributeSet = EntombedPlayerState->GetAttributeSet();

	if (AEntombedPlayerController* EntombedPlayerController = Cast<AEntombedPlayerController>(GetController()))
	{
		if (AEntombedHUD* EntombedHUD = Cast<AEntombedHUD>(EntombedPlayerController->GetHUD()))
		{
			EntombedHUD->InitializeOverlay(EntombedPlayerController, EntombedPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();

	if (HasAuthority())
	{
		UEntombedAbilitySystemLibrary::GrantDefaultAbilities(this, AbilitySystemComponent, Archetype);
		Cast<UEntombedAbilitySystemComponent>(AbilitySystemComponent)->OnGrantedAbilities();
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
