// Copyright Yooshley

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointCollection.generated.h"

UCLASS()
class ENTOMBED_API APointCollection : public AActor
{
	GENERATED_BODY()
	
public:	
	APointCollection();

	UFUNCTION(BlueprintPure)
	TArray<USceneComponent*> GetGroundPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride = 0);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<USceneComponent*> ImmutablePoints;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Point_0;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Point_1;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Point_2;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Point_3;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Point_4;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Point_5;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Point_6;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Point_7;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Point_8;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Point_9;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USceneComponent> Point_10;
};
