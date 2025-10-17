// Copyright Yooshley


#include "Character/EntombedPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/EntombedPlayerState.h"

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
	InitAbilityActorInfo();
}

void AEntombedPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// init ability actor info for the client
	InitAbilityActorInfo();
}

void AEntombedPlayerCharacter::InitAbilityActorInfo()
{
	AEntombedPlayerState* EntombedPlayerState = GetPlayerState<AEntombedPlayerState>();
	check(EntombedPlayerState);
	
	EntombedPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(EntombedPlayerState, this);
	AbilitySystemComponent = EntombedPlayerState->GetAbilitySystemComponent();
	AttributeSet = EntombedPlayerState->GetAttributeSet();
}
