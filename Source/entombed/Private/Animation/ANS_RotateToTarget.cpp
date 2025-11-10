// Copyright Yooshley


#include "Animation/ANS_RotateToTarget.h"

#include "Interaction/CombatInterface.h"

void UANS_RotateToTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	NotifyDuration = TotalDuration;
	ElapsedTime = 0.f;

	OwnerActor = MeshComp->GetOwner();
	if (OwnerActor && OwnerActor->Implements<UCombatInterface>())
	{
		InitialRotation = OwnerActor->GetActorRotation();
		TargetRotation = ICombatInterface::Execute_GetTargetDirection(OwnerActor);
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
	OwnerActor->SetActorRotation(NewRotation);
}

void UANS_RotateToTarget::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	OwnerActor->SetActorRotation(TargetRotation);
}


