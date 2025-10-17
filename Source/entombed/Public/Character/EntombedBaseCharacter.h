// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "EntombedBaseCharacter.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

UCLASS(Abstract)
class ENTOMBED_API AEntombedBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEntombedBaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY()
	TArray<TObjectPtr<USkeletalMeshComponent>> EquipmentSlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment|Body")
	TObjectPtr<USkeletalMeshComponent> HeadEquipment;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment|Body")
	TObjectPtr<USkeletalMeshComponent> TorsoEquipment;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment|Body")
	TObjectPtr<USkeletalMeshComponent> ArmLeftEquipment;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment|Body")
	TObjectPtr<USkeletalMeshComponent> ArmRightEquipment;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment|Body")
	TObjectPtr<USkeletalMeshComponent> LegLeftEquipment;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment|Body")
	TObjectPtr<USkeletalMeshComponent> LegRightEquipment;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment|Attachment")
	TObjectPtr<USkeletalMeshComponent> HeadAttachment;

	UPROPERTY(EditAnywhere, Category="Equipment|Item")
	TObjectPtr<USkeletalMeshComponent> MainHandItem;

	UPROPERTY(EditAnywhere, Category="Equipment|Item")
	TObjectPtr<USkeletalMeshComponent> OffHandItem;

private:
	UFUNCTION()
	USkeletalMeshComponent* CreateEquipmentSlot(const FName& Name);
	
	UFUNCTION()
	void SetupDefaultEquipment();
};
