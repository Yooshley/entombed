// Copyright Yooshley


#include "Character/EntombedPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/EntombedPlayerController.h"
#include "Player/EntombedPlayerState.h"
#include "UI/HUD/EntombedHUD.h"

AEntombedPlayerCharacter::AEntombedPlayerCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AEntombedPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// init ability actor info for the server
	InitializeAbilityActorInfo();
	AddDefaultAbilities();
}

void AEntombedPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// init ability actor info for the client
	InitializeAbilityActorInfo();
}

int32 AEntombedPlayerCharacter::GetCharacterLevel()
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
}
