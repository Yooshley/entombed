// Copyright Yooshley


#include "Player/EntombedPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "Input/EntombedInputComponent.h"
#include "Interaction/TargetInterface.h"

AEntombedPlayerController::AEntombedPlayerController()
{
	bReplicates = true;
}

void AEntombedPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AEntombedPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(PlayerContext);

	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AEntombedPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEntombedInputComponent* EntombedInputComponent = CastChecked<UEntombedInputComponent>(InputComponent);
	EntombedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEntombedPlayerController::Move);
	EntombedInputComponent->BindAbilityActions(InputDataAsset, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AEntombedPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlPawn = GetPawn<APawn>())
	{
		ControlPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AEntombedPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	if (ThisActor != LastActor)
	{
		if (LastActor != nullptr)
		{
			LastActor->UnHighlightActor();
		}
 
		if (ThisActor != nullptr)
		{
			ThisActor->HighlightActor();
		}
	}
}

void AEntombedPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
}

void AEntombedPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetEntombedAbilitySystemComponent() == nullptr) return;
	GetEntombedAbilitySystemComponent()->AbilityInputReleased(InputTag);
}

void AEntombedPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetEntombedAbilitySystemComponent() == nullptr) return;
	GetEntombedAbilitySystemComponent()->AbilityInputHeld(InputTag);
}

UEntombedAbilitySystemComponent* AEntombedPlayerController::GetEntombedAbilitySystemComponent()
{
	if (EntombedAbilitySystemComponent == nullptr)
	{
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>());
		EntombedAbilitySystemComponent = Cast<UEntombedAbilitySystemComponent>(ASC);
	}
	return EntombedAbilitySystemComponent;
}
