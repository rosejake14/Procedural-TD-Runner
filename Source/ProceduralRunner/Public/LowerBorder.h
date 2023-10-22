// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LowerBorder.generated.h"

class UBoxComponent;

UCLASS()
class PROCEDURALRUNNER_API ALowerBorder : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Border;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneRoot = nullptr;
public:	
	// Sets default values for this actor's properties
	ALowerBorder();

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep,
							  const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
