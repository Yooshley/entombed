// Copyright Yooshley


#include "AbilitySystem/EntombedAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UEntombedAttributeSet::UEntombedAttributeSet()
{
}

void UEntombedAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, Life, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, MaxLife, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, Form, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, MaxForm, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, Mind, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, MaxMind, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, Instinct, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, Technique, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, Acumen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, Logic, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, Spirit, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, LifeRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, FormRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, MindRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, CriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, EvasionChance, COND_None, REPNOTIFY_Always);
}

void UEntombedAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Properties;
	SetEffectProperties(Data, Properties);

	if (Data.EvaluatedData.Attribute == GetLifeAttribute())
	{
		SetLife(FMath::Clamp(GetLife(), 0.f, GetMaxLife()));
	}
	if (Data.EvaluatedData.Attribute == GetMindAttribute())
	{
		SetMind(FMath::Clamp(GetMind(), 0.f, GetMaxMind()));
	}
}

void UEntombedAttributeSet::OnRep_Life(const FGameplayAttributeData& OldLife) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, Life, OldLife);
}

void UEntombedAttributeSet::OnRep_MaxLife(const FGameplayAttributeData& OldMaxLife) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, MaxLife, OldMaxLife);
}

void UEntombedAttributeSet::OnRep_Form(const FGameplayAttributeData& OldGrit) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, Form, OldGrit);
}

void UEntombedAttributeSet::OnRep_MaxForm(const FGameplayAttributeData& OldMaxGrit) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, MaxForm, OldMaxGrit);
}

void UEntombedAttributeSet::OnRep_Mind(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, Mind, OldMana);
}

void UEntombedAttributeSet::OnRep_MaxMind(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, MaxMind, OldMaxMana);
}

void UEntombedAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, Vigor, OldVigor);
}

void UEntombedAttributeSet::OnRep_Instinct(const FGameplayAttributeData& OldInstinct) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, Instinct, OldInstinct);
}

void UEntombedAttributeSet::OnRep_Technique(const FGameplayAttributeData& OldTechnique) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, Logic, OldTechnique);
}

void UEntombedAttributeSet::OnRep_Acumen(const FGameplayAttributeData& OldAcumen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, Acumen, OldAcumen);
}

void UEntombedAttributeSet::OnRep_Logic(const FGameplayAttributeData& OldLogic) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, Technique, OldLogic);
}

void UEntombedAttributeSet::OnRep_Spirit(const FGameplayAttributeData& OldSpirit) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, Spirit, OldSpirit);
}

void UEntombedAttributeSet::OnRep_LifeRegeneration(const FGameplayAttributeData& OldLifeRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, LifeRegeneration, OldLifeRegeneration);
}

void UEntombedAttributeSet::OnRep_FormRegeneration(const FGameplayAttributeData& OldFormRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, FormRegeneration, OldFormRegeneration);
}

void UEntombedAttributeSet::OnRep_MindRegeneration(const FGameplayAttributeData& OldMindRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, MindRegeneration, OldMindRegeneration);
}

void UEntombedAttributeSet::OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, CriticalChance, OldCriticalChance);
}

void UEntombedAttributeSet::OnRep_EvasionChance(const FGameplayAttributeData& OldEvasionChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, EvasionChance, OldEvasionChance);
}

void UEntombedAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Properties) const
{
	
	Properties.EffectContextHandle = Data.EffectSpec.GetContext();
	Properties.SourceAbilitySystemComponent = Properties.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Properties.SourceAbilitySystemComponent))
	{
		Properties.SourceAvatarActor = Properties.SourceAbilitySystemComponent->GetAvatarActor();
		Properties.SourceController = Properties.SourceAbilitySystemComponent->AbilityActorInfo->PlayerController.Get();
		if (Properties.SourceController == nullptr && Properties.SourceAvatarActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(Properties.SourceAvatarActor))
			{
				Properties.SourceController = Pawn->GetController();
			}
		}
		if (Properties.SourceController)
		{
			Properties.SourceCharacter = Cast<ACharacter>(Properties.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid())
	{
		Properties.TargetAvatarActor = Data.Target.GetAvatarActor();
		Properties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Properties.TargetCharacter = Cast<ACharacter>(Properties.TargetController->GetPawn());
		Properties.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Properties.TargetAvatarActor);
	}
}
