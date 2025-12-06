// Copyright Yooshley


#include "Animation/ANS_RotateToTarget.h"

#include "entombed/Public/Interface/CombatInterface.h"

void UANS_RotateToTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	NotifyDuration = TotalDuration;
	ElapsedTime = 0.f;

	OwnerActor = MeshComp->GetOwner();
	if (OwnerActor.IsValid() && OwnerActor->Implements<UCombatInterface>())
	{
		InitialRotation = OwnerActor->GetActorRotation();
		TargetRotation = ICombatInterface::Execute_GetTargetDirection(OwnerActor.Get());
	}
}

void UANS_RotateToTarget::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	ElapsedTime += FrameDeltaTime;
	
	float Alpha = FMath::Clamp(ElapsedTime / NotifyDuration, 0.f, 1.f);

	FRotator FinalRotation = TargetRotation;

	FinalRotation.Pitch = InitialRotation.Pitch;
	FinalRotation.Roll = InitialRotation.Roll;
	
	FRotator NewRotation = FMath::Lerp(InitialRotation, FinalRotation, Alpha);
	if (OwnerActor.IsValid()) OwnerActor->SetActorRotation(NewRotation);
}

void UANS_RotateToTarget::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (OwnerActor.IsValid()) OwnerActor->SetActorRotation(TargetRotation);
}


