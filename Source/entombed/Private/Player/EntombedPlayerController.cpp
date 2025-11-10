// Copyright Yooshley


#include "Player/EntombedPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EntombedGameplayTags.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Input/EntombedInputComponent.h"
#include "Interaction/TargetInterface.h"
#include "UI/Widget/DamageTextComponent.h"

AEntombedPlayerController::AEntombedPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AEntombedPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	if (bClickToMove)
	{
		AutoRun();
	}
}

void AEntombedPlayerController::ShowDamageNumber_Implementation(float Damage, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(Damage, bBlockedHit, bCriticalHit);
	}
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

void AEntombedPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AEntombedPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AEntombedPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEntombedInputComponent* EntombedInputComponent = CastChecked<UEntombedInputComponent>(InputComponent);
	EntombedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEntombedPlayerController::Move);
	EntombedInputComponent->BindAction(ModifierAction, ETriggerEvent::Started, this, &AEntombedPlayerController::ModifierPressed);
	EntombedInputComponent->BindAction(ModifierAction, ETriggerEvent::Completed, this, &AEntombedPlayerController::ModifierReleased);
	EntombedInputComponent->BindAbilityActions(InputDataAsset, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AEntombedPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlPawn = GetPawn<APawn>())
	{
		ControlPawn->AddMovementInput(Forward, InputAxisVector.Y);
		ControlPawn->AddMovementInput(Right, InputAxisVector.X);
	}
}

void AEntombedPlayerController::AutoRun()
{
	if (!bAutoRunning) return;

	if (APawn* ControlPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AEntombedPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	if (ThisActor != LastActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}
}

void AEntombedPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FEntombedGameplayTags::Get().Input_MainHand))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void AEntombedPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FEntombedGameplayTags::Get().Input_MainHand))
	{
		if (GetEntombedASC()) GetEntombedASC()->AbilityInputReleased(InputTag);
		return;
	}
	
	if (GetEntombedASC()) GetEntombedASC()->AbilityInputReleased(InputTag);

	return;
	
	if (!bTargeting && !bModifierRunning && bClickToMove)
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			const FVector Start = ControlledPawn->GetActorLocation();
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, Start, CachedDestination))
			{
				Spline->ClearSplinePoints();
				const int32 NumPts = NavPath->PathPoints.Num();
				if (NumPts > 0)
				{
					for (const FVector& P : NavPath->PathPoints)
					{
						Spline->AddSplinePoint(P, ESplineCoordinateSpace::World);
					}
					CachedDestination = NavPath->PathPoints.Last();
					bAutoRunning = true;
				}
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AEntombedPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FEntombedGameplayTags::Get().Input_MainHand))
	{
		if(GetEntombedASC()) GetEntombedASC()->AbilityInputHeld(InputTag);
		return;
	}

	if(GetEntombedASC()) GetEntombedASC()->AbilityInputHeld(InputTag);

	return;

	if (bTargeting || bModifierRunning)
	{
		if(GetEntombedASC()) GetEntombedASC()->AbilityInputHeld(InputTag);
	}
	else
	{
		if (bClickToMove)
		{
			FollowTime += GetWorld()->GetDeltaSeconds();
			if (CursorHit.bBlockingHit)
			{
				CachedDestination = CursorHit.ImpactPoint;
			}

			if (APawn* ControlPawn = GetPawn<APawn>())
			{
				const FVector WorldDirection = (CachedDestination - ControlPawn->GetActorLocation()).GetSafeNormal();
				ControlPawn->AddMovementInput(WorldDirection);
			}
		}
	}
}

UEntombedAbilitySystemComponent* AEntombedPlayerController::GetEntombedASC()
{
	if (EntombedAbilitySystemComponent == nullptr)
	{
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>());
		EntombedAbilitySystemComponent = Cast<UEntombedAbilitySystemComponent>(ASC);
	}
	return EntombedAbilitySystemComponent;
}
