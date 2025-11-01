// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderCursor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCursorTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
 * 
 */
UCLASS()
class ENTOMBED_API UTargetDataUnderCursor : public UAbilityTask
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Ability|Task", meta=(HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly=true))
	static UTargetDataUnderCursor* CreateTargetDataUnderCursor(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FCursorTargetDataSignature ValidData;

private:
	virtual void Activate() override;
	void SendCursorData();
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
