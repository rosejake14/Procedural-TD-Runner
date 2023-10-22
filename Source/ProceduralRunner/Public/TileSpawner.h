// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileSpawner.generated.h"


UCLASS()
class PROCEDURALRUNNER_API ATileSpawner : public AActor
{
	GENERATED_BODY()

	
	//UPROPERTY(EditAnywhere)
	//	TSubclassOf<AActor> Ground;
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<AActor>> TilesToSpawn;

	

	
public:	
	// Sets default values for this actor's properties
	ATileSpawner();
	UFUNCTION(BlueprintCallable)
		void SpawnTile();
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<AActor>> SpawnedTiles;
	UPROPERTY(EditAnywhere)
		int32 numberOfTilesSpawned;

	//UFUNCTION(BlueprintCallable)
	//static ATileSpawner* Get();

private:
	static TSharedPtr<ATileSpawner> STileSpawner;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
