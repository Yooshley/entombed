// Copyright Yooshley


#include "Niagara/PassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "entombed/Public/Interface/CombatInterface.h"

UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UEntombedAbilitySystemComponent* EntombedASC = Cast<UEntombedAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		EntombedASC->ActivatePassiveEffectDelegate.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
	}
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnAbilitySystemReadyDelegate().AddLambda([this](UAbilitySystemComponent* AbilitySystemComponent)
		{
			if (UEntombedAbilitySystemComponent* EntombedASC = Cast<UEntombedAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
			{
				EntombedASC->ActivatePassiveEffectDelegate.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
			}
		});
	}
}

void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate)
{
	if (AbilityTag.MatchesTagExact(PassiveAbilityTag))
	{
		if (bActivate && !IsActive())
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}
