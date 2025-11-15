// Copyright Yooshley


#include "AbilitySystem/AsyncTask/AwaitCooldownChange.h"

#include "AbilitySystemComponent.h"

UAwaitCooldownChange* UAwaitCooldownChange::AwaitCooldownChange(UAbilitySystemComponent* InAbilitySystemComponent,
                                                                const FGameplayTag& InCooldownTag)
{
	if (!IsValid(InAbilitySystemComponent) || !InCooldownTag.IsValid()) return nullptr;
	
	UAwaitCooldownChange* AwaitCooldownChange = NewObject<UAwaitCooldownChange>();
	AwaitCooldownChange->AbilitySystemComponent = InAbilitySystemComponent;
	AwaitCooldownChange->CooldownTag = InCooldownTag;

	//cooldown removed
	InAbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(AwaitCooldownChange, &UAwaitCooldownChange::CooldownTagChanged);

	//cooldown added
	InAbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(AwaitCooldownChange, &UAwaitCooldownChange::OnActiveEffectAdded);

	return AwaitCooldownChange;
}

void UAwaitCooldownChange::EndTask()
{
	if (!IsValid(AbilitySystemComponent)) return;
	AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UAwaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UAwaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& Spec,
	FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTags;
	Spec.GetAllAssetTags(AssetTags);
	
	FGameplayTagContainer GrantedTags;
	Spec.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		if (TimesRemaining.Num() > 0)
		{
			const float TimeRemaining = FMath::Max(TimesRemaining);
			CooldownBegin.Broadcast(TimeRemaining);
		}
	}
}
