// Copyright Yooshley


#include "AbilitySystem/EntombedAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "EntombedAbilityTypes.h"
#include "EntombedGameplayTags.h"
#include "AbilitySystem/EntombedAbilitySystemLibrary.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "GameFramework/Character.h"
#include "entombed/Public/Interface/CombatInterface.h"
#include "entombed/Public/Interface/PlayerInterface.h"
#include "Net/UnrealNetwork.h"
#include "Player/EntombedPlayerController.h"

UEntombedAttributeSet::UEntombedAttributeSet()
{
}

void UEntombedAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, Life, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, TotalLife, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, LifeRegenerationRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, LifeRegenerationDelay, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, Form, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, TotalForm, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, FormRegenerationRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, FormRegenerationDelay, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, CriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, CriticalMultiplier, COND_None, REPNOTIFY_Always)
	
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, BurnResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, ShockResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, FreezeResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEntombedAttributeSet, ElementalResistance, COND_None, REPNOTIFY_Always);
}

void UEntombedAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Properties;
	SetEffectProperties(Data, Properties);
	
	if (Properties.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Properties.TargetCharacter)) return;

	if (Data.EvaluatedData.Attribute == GetLifeAttribute())
	{
		SetLife(FMath::Clamp(GetLife(), 0.f, GetTotalLife()));
	}

	if (Data.EvaluatedData.Attribute == GetFormAttribute())
	{
		SetForm(FMath::Clamp(GetForm(), 0.f, GetTotalForm()));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Properties);
	}

	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Properties);
	}
}

void UEntombedAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetTotalLifeAttribute() && bMaxOutLife)
	{
		SetLife(GetTotalLife());
		bMaxOutLife = false;
	}
	if (Attribute == GetTotalFormAttribute() && bMaxOutForm)
	{
		SetForm(GetTotalForm());
		bMaxOutForm = false;
	}
}

void UEntombedAttributeSet::HandleIncomingDamage(const FEffectProperties& Properties)
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
			SendXPEvent(Properties);
		}
		// else
		// {
		// 	FGameplayTagContainer TagContainer;
		// 	TagContainer.AddTag(FEntombedGameplayTags::Get().Effect_Knockback);
		// 	Properties.TargetAbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
		//
		// 	const FVector& KnockbackVector = FVector::ZeroVector;
		// 	if (!KnockbackVector.IsNearlyZero(1.f))
		// 	{
		// 		Properties.TargetCharacter->LaunchCharacter(KnockbackVector, true, false);
		// 	}
		// }
		const bool bBlocked = false; //TODO: Implement block based on active ability
		const bool bCritical = UEntombedAbilitySystemLibrary::IsCriticalHit(Properties.EffectContextHandle);
		ShowFloatingText(Properties, LocalIncomingDamage, bBlocked, bCritical);
		if (UEntombedAbilitySystemLibrary::HasDebuffs(Properties.EffectContextHandle))
		{
			HandleIncomingDebuff(Properties);
		}
	}
}

void UEntombedAttributeSet::HandleIncomingXP(const FEffectProperties& Properties)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0);

	if (Properties.SourceCharacter->Implements<UPlayerInterface>() && Properties.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetCharacterLevel(Properties.SourceCharacter);
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Properties.SourceCharacter);
		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Properties.SourceCharacter, CurrentXP + LocalIncomingXP);
		const int32 NumLevelUps = NewLevel - CurrentLevel;
		if (NumLevelUps > 0)
		{
			int32 AttributePointsAward = 0;
			int32 AbilityPointsAward = 0;
			for (int32 i = 0; i < NumLevelUps; i++)
			{
				AttributePointsAward += IPlayerInterface::Execute_GetAttributePointsAward(Properties.SourceCharacter, CurrentLevel+i);
				AbilityPointsAward += IPlayerInterface::Execute_GetAbilityPointsAward(Properties.SourceCharacter, CurrentLevel+i);
			}
			IPlayerInterface::Execute_AddLevel(Properties.SourceCharacter, NumLevelUps);
			IPlayerInterface::Execute_LevelUp(Properties.SourceCharacter);
			IPlayerInterface::Execute_AddAttributePoints(Properties.SourceCharacter, AttributePointsAward);
			IPlayerInterface::Execute_AddAbilityPoints(Properties.SourceCharacter, AbilityPointsAward);

			bMaxOutLife = true;
			bMaxOutForm = true;
			bMaxOutMind = true;
		}
			
		IPlayerInterface::Execute_AddXP(Properties.SourceCharacter, LocalIncomingXP);
	}
}

void UEntombedAttributeSet::HandleIncomingDebuff(const FEffectProperties& Properties)
{
	//TODO: switch to ChanceToApplyGameplayEffect/CustomApplyGameplayEffectComponent to handle debuff/DoT effects
	const FEntombedGameplayTags& GameplayTags = FEntombedGameplayTags::Get();
	
	const TArray<FAppliedDebuffData> AppliedDebuffs = UEntombedAbilitySystemLibrary::GetAppliedDebuffs(Properties.EffectContextHandle);
	if (AppliedDebuffs.Num() == 0)
	{
		return;
	}
	
	FGameplayEffectContextHandle EffectContext = Properties.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(Properties.SourceAvatarActor);
	
	for (const FAppliedDebuffData& DebuffData : AppliedDebuffs)
	{
		const FGameplayTag& DebuffType = DebuffData.DebuffType;
		const float DebuffDamage = DebuffData.Damage;
		const float DebuffDuration = DebuffData.Duration;
		const float DebuffPeriod = DebuffData.Period;
		
		const FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DebuffType.ToString());
		UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));
		
		Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
		Effect->Period = FScalableFloat(DebuffPeriod);
		Effect->DurationMagnitude = FScalableFloat(DebuffDuration);
		Effect->bExecutePeriodicEffectOnApplication = false;

		UTargetTagsGameplayEffectComponent& Component = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
		
		if (const FGameplayTag* DebuffTag = GameplayTags.DamageTypesToDebuffs.Find(DebuffType))
		{
			FInheritedTagContainer TagContainer;
			TagContainer.Added.AddTag(*DebuffTag);
			
			if (DebuffTag->MatchesTagExact(GameplayTags.Debuff_Shock))
			{
				TagContainer.Added.AddTag(GameplayTags.Player_Block_InputPressed);
				TagContainer.Added.AddTag(GameplayTags.Player_Block_InputHeld);
				TagContainer.Added.AddTag(GameplayTags.Player_Block_InputReleased);
			}
			Component.SetAndApplyTargetTagChanges(TagContainer);
			
			//Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
			if (DebuffData.bCanStack)
			{
				Effect->StackLimitCount = DebuffData.MaxStacks;
			}
			
			if (DebuffDamage > 0.f)
			{
				const int32 Index = Effect->Modifiers.Add(FGameplayModifierInfo());
				FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];

				ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
				ModifierInfo.ModifierOp = EGameplayModOp::Additive;
				ModifierInfo.Attribute = GetIncomingDamageAttribute();
			}
			
			FGameplayEffectSpec SpecToApply(Effect, EffectContext, 1.f);
			Properties.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(SpecToApply);
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

void UEntombedAttributeSet::OnRep_LifeRegenerationRate(const FGameplayAttributeData& OldLifeRegenerationRate) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, LifeRegenerationRate, OldLifeRegenerationRate);
}

void UEntombedAttributeSet::OnRep_LifeRegenerationDelay(const FGameplayAttributeData& OldLifeRegenerationDelay) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, LifeRegenerationDelay, OldLifeRegenerationDelay);
}

void UEntombedAttributeSet::OnRep_Form(const FGameplayAttributeData& OldForm) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, Form, OldForm);
}

void UEntombedAttributeSet::OnRep_TotalForm(const FGameplayAttributeData& OldTotalForm) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, TotalForm, OldTotalForm);
}

void UEntombedAttributeSet::OnRep_FormRegenerationRate(const FGameplayAttributeData& OldFormRegenerationRate) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, FormRegenerationRate, OldFormRegenerationRate);
}

void UEntombedAttributeSet::OnRep_FormRegenerationDelay(const FGameplayAttributeData& OldFormRegenerationDelay) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, FormRegenerationDelay, OldFormRegenerationDelay);
}

void UEntombedAttributeSet::OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, CriticalChance, OldCriticalChance);
}

void UEntombedAttributeSet::OnRep_CriticalMultiplier(const FGameplayAttributeData& OldCriticalMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, CriticalMultiplier, OldCriticalMultiplier);
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

void UEntombedAttributeSet::OnRep_ElementalResistance(const FGameplayAttributeData& OldElementalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEntombedAttributeSet, ElementalResistance, OldElementalResistance);
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

void UEntombedAttributeSet::SendXPEvent(const FEffectProperties& Props) const
{
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		int32 TargetLevel = ICombatInterface::Execute_GetCharacterLevel(Props.TargetCharacter);
		EEntombedArchetype TargetArchetype = ICombatInterface::Execute_GetArchetype(Props.TargetCharacter);
		const int32 XPAward = UEntombedAbilitySystemLibrary::GetXPAwardForArchetype(Props.TargetCharacter, TargetArchetype, TargetLevel);

		const FEntombedGameplayTags& GameplayTags = FEntombedGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attribute_Meta_IncomingXP;
		Payload.EventMagnitude = XPAward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTags.Attribute_Meta_IncomingXP, Payload);
	}
}
