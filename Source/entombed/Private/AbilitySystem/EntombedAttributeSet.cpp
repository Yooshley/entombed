// Copyright Yooshley


#include "AbilitySystem/EntombedAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "EntombedGameplayTags.h"
#include "AbilitySystem/EntombedAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/EntombedPlayerController.h"

UEntombedAttributeSet::UEntombedAttributeSet()
{
	const FEntombedGameplayTags& GameplayTags = FEntombedGameplayTags::Get();
}

void UEntombedAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, Life, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, TotalLife, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, Form, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, TotalForm, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, Mind, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, TotalMind, COND_None, REPNOTIFY_Always);
	
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
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, CriticalMultiplier, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, EvadeChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, ArmorRating, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, BurnResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, ShockResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, FreezeResistance, COND_None, REPNOTIFY_Always);
}

void UEntombedAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Properties;
	SetEffectProperties(Data, Properties);

	if (Data.EvaluatedData.Attribute == GetLifeAttribute())
	{
		SetLife(FMath::Clamp(GetLife(), 0.f, GetTotalLife()));
		UE_LOG(LogTemp, Warning, TEXT("Life Changed on %s, New Life: %f"), *Properties.TargetAvatarActor->GetName(), GetLife());
	}

	if (Data.EvaluatedData.Attribute == GetFormAttribute())
	{
		SetForm(FMath::Clamp(GetForm(), 0.f, GetTotalForm()));
	}
	
	if (Data.EvaluatedData.Attribute == GetMindAttribute())
	{
		SetMind(FMath::Clamp(GetMind(), 0.f, GetTotalMind()));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0);
		if (LocalIncomingDamage >= 0.f)
		{
			const float NewLife = GetLife() - LocalIncomingDamage;
			SetLife(FMath::Clamp(NewLife, 0.f, GetTotalLife()));

			const bool bFatal = NewLife <= 0.f;
			if (bFatal)
			{
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(Properties.TargetAvatarActor);
				if (CombatInterface)
				{
					CombatInterface->Death();
				}
			}
			else
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FEntombedGameplayTags::Get().Effect_Knockback);
				Properties.TargetAbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
			}
			const bool bBlocked = UEntombedAbilitySystemLibrary::IsBlockedHit(Properties.EffectContextHandle);
			const bool bCritical = UEntombedAbilitySystemLibrary::IsCriticalHit(Properties.EffectContextHandle);
			ShowFloatingText(Properties, LocalIncomingDamage, bBlocked, bCritical);
		}
	}
}

void UEntombedAttributeSet::OnRep_Life(const FGameplayAttributeData& OldLife) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, Life, OldLife);
}

void UEntombedAttributeSet::OnRep_TotalLife(const FGameplayAttributeData& OldTotalLife) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, TotalLife, OldTotalLife);
}

void UEntombedAttributeSet::OnRep_Form(const FGameplayAttributeData& OldForm) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, Form, OldForm);
}

void UEntombedAttributeSet::OnRep_TotalForm(const FGameplayAttributeData& OldTotalForm) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, TotalForm, OldTotalForm);
}

void UEntombedAttributeSet::OnRep_Mind(const FGameplayAttributeData& OldMind) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, Mind, OldMind);
}

void UEntombedAttributeSet::OnRep_TotalMind(const FGameplayAttributeData& OldTotalMind) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, TotalMind, OldTotalMind);
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

void UEntombedAttributeSet::OnRep_CriticalMultiplier(const FGameplayAttributeData& OldCriticalMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, CriticalMultiplier, OldCriticalMultiplier);
}

void UEntombedAttributeSet::OnRep_EvadeChance(const FGameplayAttributeData& OldEvadeChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, EvadeChance, OldEvadeChance);
}

void UEntombedAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, BlockChance, OldBlockChance);
}

void UEntombedAttributeSet::OnRep_ArmorRating(const FGameplayAttributeData& OldArmorRating) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, ArmorRating, OldArmorRating);
}

void UEntombedAttributeSet::OnRep_BurnResistance(const FGameplayAttributeData& OldBurnResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, BurnResistance, OldBurnResistance);
}

void UEntombedAttributeSet::OnRep_ShockResistance(const FGameplayAttributeData& OldShockResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, ShockResistance, OldShockResistance);
}

void UEntombedAttributeSet::OnRep_FreezeResistance(const FGameplayAttributeData& OldFreezeResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, FreezeResistance, OldFreezeResistance);
}

void UEntombedAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Properties) const
{
	
	Properties.EffectContextHandle = Data.EffectSpec.GetContext();
	Properties.SourceASC = Properties.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Properties.SourceASC) && Properties.SourceASC->AbilityActorInfo.IsValid() && Properties.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Properties.SourceAvatarActor = Properties.SourceASC->GetAvatarActor();
		Properties.SourceController = Properties.SourceASC->AbilityActorInfo->PlayerController.Get();
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
		Properties.TargetCharacter = Cast<ACharacter>(Properties.TargetAvatarActor);
		Properties.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Properties.TargetAvatarActor);
	}
}

const void UEntombedAttributeSet::ShowFloatingText(const FEffectProperties& Properties, float Damage, bool bBlockedHit, bool bCriticalHit)
{
	if (Properties.SourceCharacter != Properties.TargetCharacter)
	{
		if (AEntombedPlayerController* EntombedPC = Cast<AEntombedPlayerController>(Properties.SourceCharacter->GetController())) //dealing damage
		{
			EntombedPC->ShowDamageNumber(Damage, Properties.TargetCharacter, bBlockedHit, bCriticalHit);
			return;
		}
		if (AEntombedPlayerController* EntombedPC = Cast<AEntombedPlayerController>(Properties.TargetCharacter->GetController())) //taking damage
		{
			EntombedPC->ShowDamageNumber(Damage, Properties.TargetCharacter, bBlockedHit, bCriticalHit);
		}
	}
}
