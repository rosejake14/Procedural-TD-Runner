// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileSpawner.h"
#include "GameFramework/Actor.h"
#include "ProceduralRunner/Public/TileSpawner.h"
#include "Floor.generated.h"

class UBoxComponent;
class ATileSpawner;

UCLASS()
class PROCEDURALRUNNER_API AFloor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* EndCollision;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* ObstacleCollision;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneRoot = nullptr;
	
public:	
	// Sets default values for this actor's properties
	AFloor();

	UPROPERTY(EditAnywhere)
	ATileSpawner* ts;
		
	UFUNCTION()
	virtual auto OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep,
	                          const FHitResult& SweepResult) -> void;

	UFUNCTION()
	virtual auto OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) -> void;

	UFUNCTION()
	virtual auto OnObstacleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep,
	                               const FHitResult& SweepResult) -> void;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
