// Copyright Yooshley


#include "Player/EntombedPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EntombedGameplayTags.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Character/EntombedPlayerCharacter.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "entombed/entombed.h"
#include "GameFramework/Character.h"
#include "Input/EntombedInputComponent.h"
#include "entombed/Public/Interface/TargetInterface.h"
#include "Interface/HostileInterface.h"
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

	UpdateMagicCircleLocation();
}

void AEntombedPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if (DecalMaterial)
		{
			MagicCircle->DecalComponent->SetMaterial(0, DecalMaterial);
		}
		bShowMouseCursor = false;
	}
}

void AEntombedPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
	bShowMouseCursor = true;
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

    if (AEntombedPlayerCharacter* PC = GetPawn<AEntombedPlayerCharacter>())
    {
        const FRotator CamRot = PC->GetCameraBoomRotation();
        const FRotator YawRotation(0.f, CamRot.Yaw, 0.f);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        PC->AddMovementInput(ForwardDirection, InputAxisVector.Y);
        PC->AddMovementInput(RightDirection,   InputAxisVector.X);
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

void AEntombedPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
	}
}

void AEntombedPlayerController::CursorTrace()
{
	//check for blocking tag
	if (GetEntombedASC() && GetEntombedASC()->HasMatchingGameplayTag(FEntombedGameplayTags::Get().Player_Block_CursorTrace))
	{
		UnhighlightActor(LastActor);
		UnhighlightActor(ThisActor);
		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}

	const ECollisionChannel TraceChannel = IsValid(MagicCircle) ? ECC_ExcludeActors : ECC_Visibility;
	GetHitResultUnderCursor(TraceChannel, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	if (IsValid(CursorHit.GetActor()) && CursorHit.GetActor()->Implements<UTargetInterface>())
	{
		ThisActor = CursorHit.GetActor();
	}
	else
	{
		ThisActor = nullptr;
	}

	if (ThisActor != LastActor)
	{
		UnhighlightActor(LastActor);
		HighlightActor(ThisActor);
	}
}

void AEntombedPlayerController::HighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UTargetInterface>())
	{
		ITargetInterface::Execute_HighlightActor(InActor);
	}
}

void AEntombedPlayerController::UnhighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UTargetInterface>())
	{
		ITargetInterface::Execute_UnHighlightActor(InActor);
	}
}

void AEntombedPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//check for blocking tag
	if (GetEntombedASC() && GetEntombedASC()->HasMatchingGameplayTag(FEntombedGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	
	if (InputTag.MatchesTagExact(FEntombedGameplayTags::Get().Input_Ability_MainHand_1))
	{
		if (IsValid(ThisActor))
		{
			TargetingStatus = ThisActor->Implements<UHostileInterface>() ? ETargetingStatus::Hostile : ETargetingStatus::Object;
			bAutoRunning = false;
		}
		else
		{
			TargetingStatus = ETargetingStatus::Nothing;	
		}
	}
	if (GetEntombedASC()) GetEntombedASC()->AbilityInputPressed(InputTag);
}

void AEntombedPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	//check for blocking tag
	if (GetEntombedASC() && GetEntombedASC()->HasMatchingGameplayTag(FEntombedGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	
	if (!InputTag.MatchesTagExact(FEntombedGameplayTags::Get().Input_Ability_MainHand_1))
	{
		if (GetEntombedASC()) GetEntombedASC()->AbilityInputReleased(InputTag);
		return;
	}
	
	if (GetEntombedASC()) GetEntombedASC()->AbilityInputReleased(InputTag);

	//return;
	
	// if (TargetingStatus != ETargetingStatus::Hostile && !bModifierRunning && bClickToMove)
	// {
	// 	const APawn* ControlledPawn = GetPawn();
	// 	if (FollowTime <= ShortPressThreshold && ControlledPawn)
	// 	{
	// 		const FVector Start = ControlledPawn->GetActorLocation();
	// 		if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, Start, CachedDestination))
	// 		{
	// 			Spline->ClearSplinePoints();
	// 			const int32 NumPts = NavPath->PathPoints.Num();
	// 			if (NumPts > 0)
	// 			{
	// 				for (const FVector& P : NavPath->PathPoints)
	// 				{
	// 					Spline->AddSplinePoint(P, ESplineCoordinateSpace::World);
	// 				}
	// 				CachedDestination = NavPath->PathPoints.Last();
	// 				bAutoRunning = true;
	// 			}
	// 		}
	// 	}
	// 	FollowTime = 0.f;
	// 	TargetingStatus = ETargetingStatus::Nothing;
	// }
}

void AEntombedPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//check for blocking tag
	if (GetEntombedASC() && GetEntombedASC()->HasMatchingGameplayTag(FEntombedGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	
	if (!InputTag.MatchesTagExact(FEntombedGameplayTags::Get().Input_Ability_MainHand_1))
	{
		if(GetEntombedASC()) GetEntombedASC()->AbilityInputHeld(InputTag);
		return;
	}

	if(GetEntombedASC()) GetEntombedASC()->AbilityInputHeld(InputTag);

	//return;

	// if (TargetingStatus == ETargetingStatus::Hostile || bModifierRunning)
	// {
	// 	if(GetEntombedASC()) GetEntombedASC()->AbilityInputHeld(InputTag);
	// }
	// else
	// {
	// 	if (bClickToMove)
	// 	{
	// 		FollowTime += GetWorld()->GetDeltaSeconds();
	// 		if (CursorHit.bBlockingHit)
	// 		{
	// 			CachedDestination = CursorHit.ImpactPoint;
	// 		}
	//
	// 		if (APawn* ControlPawn = GetPawn<APawn>())
	// 		{
	// 			const FVector WorldDirection = (CachedDestination - ControlPawn->GetActorLocation()).GetSafeNormal();
	// 			ControlPawn->AddMovementInput(WorldDirection);
	// 		}
	// 	}
	// }
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
