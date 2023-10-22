// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralRunner\Public\TileSpawner.h"
#include "ProceduralRunner\Public\Floor.h"




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
		int32 rX = 0;
		int32 rZ = 0;
		if(numberOfTilesSpawned!=0)
		{
			 rX = rand()%1000 - 500;
			 rZ = rand()%400 - 200;
		}
		spawnPos.SetLocation(FVector3d(rX,numberOfTilesSpawned * 2600,rZ));
		TSubclassOf<AActor> Ground = TilesToSpawn[rand()%TilesToSpawn.Num()];
		AFloor* Temp = static_cast<AFloor*>(GetWorld()->SpawnActor<AActor>(Ground, spawnPos, spawnParameters));
		Temp->ts = this;

		//AActor* Temp2 = (AActor*)(Temp);
		SpawnedTiles.Add(Ground);
		numberOfTilesSpawned++;
		//DestroyTile();
	}
	
}

void ATileSpawner::DestroyTile()
{
	//Destroy(SpawnedTiles[numberOfTilesSpawned - 1]);
}


// //singleton def
// ATileSpawner* ATileSpawner::Get()
// {
// 	if(!STileSpawner.IsValid())
// 	{
// 		STileSpawner = MakeShareable(new ATileSpawner());
// 	}
//
// 	return STileSpawner.Get();
// }
