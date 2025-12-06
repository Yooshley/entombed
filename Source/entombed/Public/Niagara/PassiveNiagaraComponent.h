// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "PassiveNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class ENTOMBED_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
	
public:
	UPassiveNiagaraComponent();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveAbilityTag;
	
protected:
	virtual void BeginPlay() override;
	void OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate);
};
