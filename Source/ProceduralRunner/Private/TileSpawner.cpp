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

void ATileSpawner::SpawnTile() //Spawn 2 random tile's ahead.
{
	for(int i = 0; i < 2; i++) 
	{
		FActorSpawnParameters spawnParameters; 
		spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FTransform spawnPos;
		int32 rX = 0;
		int32 rZ = 0; 
		TSubclassOf<AActor> Ground = TilesToSpawn[0];
		if(numberOfTilesSpawned!=0)
		{  //Generate random coordinates for new tile to spawn.
			rX = rand()%1000 - 500;  
			rZ = rand()%400 - 200; 
			Ground = TilesToSpawn[rand()%TilesToSpawn.Num()]; //Selects random tile from array.
		}
		spawnPos.SetLocation(FVector3d(rX,numberOfTilesSpawned * 2600,rZ)); //Assigning spawn position.
		AFloor* Temp = static_cast<AFloor*>(GetWorld()->SpawnActor<AActor>(Ground, spawnPos, spawnParameters));  //Spawns tile and casts it to a temporary variable.
		Temp->ts = this;  

		SpawnedTiles.Add(Ground); 
		numberOfTilesSpawned++;

	}
	
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
