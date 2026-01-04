// Copyright Yooshley


#include "Niagara/PassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EntombedGameplayTags.h"
#include "AbilitySystem/EntombedAbilitySystemComponent.h"
#include "entombed/Public/Interface/CombatInterface.h"

UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::ActivateIfEquipped(UEntombedAbilitySystemComponent* EntombedASC)
{
	const bool bDefaultAbilitiesGranted = EntombedASC->bGrantedDefaultAbilities;
	if (bDefaultAbilitiesGranted)
	{
		if (EntombedASC->GetStatusTagFromAbilityTag(PassiveAbilityTag).MatchesTagExact(FEntombedGameplayTags::Get().Ability_Status_Equipped))
		{
			Activate();
		}
	}
}

void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UEntombedAbilitySystemComponent* EntombedASC = Cast<UEntombedAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		EntombedASC->ActivatePassiveEffectDelegate.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
		ActivateIfEquipped(EntombedASC);
	}
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnAbilitySystemReadyDelegate().AddLambda([this](UAbilitySystemComponent* AbilitySystemComponent)
		{
			if (UEntombedAbilitySystemComponent* EntombedASC = Cast<UEntombedAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
			{
				EntombedASC->ActivatePassiveEffectDelegate.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
				ActivateIfEquipped(EntombedASC);
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
