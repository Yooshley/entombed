// Copyright Yooshley


#include "Actor/EntombedEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/EntombedAttributeSet.h"
#include "Components/SphereComponent.h"


AEntombedEffectActor::AEntombedEffectActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void AEntombedEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO: Replace this with Gameplay Effect, not good practice
	if (IAbilitySystemInterface* OtherActorInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UEntombedAttributeSet* OtherActorAttributeSet = Cast<UEntombedAttributeSet>(OtherActorInterface->GetAbilitySystemComponent()->GetAttributeSet(UEntombedAttributeSet::StaticClass()));
		UEntombedAttributeSet* MutableAttributeSet = const_cast<UEntombedAttributeSet*>(OtherActorAttributeSet);
		MutableAttributeSet->SetLife(OtherActorAttributeSet->GetLife() + 25.f);
		Destroy();
	}
}

void AEntombedEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AEntombedEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AEntombedEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AEntombedEffectActor::EndOverlap);
}

