// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "EntombedPlayerController.generated.h"

class USplineComponent;
class UEntombedAbilitySystemComponent;
class UEntombedInputDataAsset;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class ITargetInterface;

/**
 * 
 */
UCLASS()
class ENTOMBED_API AEntombedPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AEntombedPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> PlayerContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ModifierAction;


	void Move(const FInputActionValue& InputActionValue);

	void ModifierPressed() {bModifierRunning = true;}
	void ModifierReleased() {bModifierRunning = false;}
	bool bModifierRunning = false;

	void CursorTrace();
	TScriptInterface<ITargetInterface> LastActor;
	TScriptInterface<ITargetInterface> ThisActor;
	FHitResult CursorHit;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UEntombedInputDataAsset> InputDataAsset;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY()
	TObjectPtr<UEntombedAbilitySystemComponent> EntombedAbilitySystemComponent;

	UEntombedAbilitySystemComponent* GetEntombedASC();

	
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();
};
