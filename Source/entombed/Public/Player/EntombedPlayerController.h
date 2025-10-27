// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "EntombedPlayerController.generated.h"

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


	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();
	TScriptInterface<ITargetInterface> LastActor;
	TScriptInterface<ITargetInterface> ThisActor;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UEntombedInputDataAsset> InputDataAsset;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY()
	TObjectPtr<UEntombedAbilitySystemComponent> EntombedAbilitySystemComponent;

	UEntombedAbilitySystemComponent* GetEntombedAbilitySystemComponent();
};
