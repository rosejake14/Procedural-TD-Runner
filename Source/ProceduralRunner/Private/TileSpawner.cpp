// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralRunner\Public\TileSpawner.h"
#include "Floor.h"




// Sets default values
ATileSpawner::ATileSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	numberOfTilesSpawned = 0;
}

// Called when the game starts or when spawned
void ATileSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnTile();
}

// Called every frame
void ATileSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATileSpawner::SpawnTile()
{
	for(int i = 0; i < 2; i++)
	{
		FActorSpawnParameters spawnParameters;
		spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FTransform spawnPos;
		spawnPos.SetLocation(FVector3d(0,numberOfTilesSpawned * 2600,0));
		TSubclassOf<AActor> Ground = TilesToSpawn[rand()%TilesToSpawn.Num()];
		AFloor* Temp = (AFloor*)GetWorld()->SpawnActor<AActor>(Ground, spawnPos, spawnParameters);
		Temp->ts = this;
		
		SpawnedTiles.Add(Ground);
		numberOfTilesSpawned++;
	}
	
}