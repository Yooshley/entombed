// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_RotateToTarget.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Rotate To Target"))
class ENTOMBED_API UANS_RotateToTarget : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rotation")
	FRotator TargetRotation = FRotator::ZeroRotator;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	FRotator InitialRotation = FRotator::ZeroRotator;
	float NotifyDuration = 0.f;
	float ElapsedTime = 0.f;
	
	UPROPERTY()
	AActor* OwnerActor = nullptr;
};
