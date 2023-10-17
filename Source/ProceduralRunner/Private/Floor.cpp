// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralRunner\Public\Floor.h"
#include "Components/BoxComponent.h"
#include "ProceduralRunner\ProceduralRunnerCharacter.h"

class ATileSpawner;

// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = SceneRoot;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	EndCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("EndCollision"));
	EndCollision->SetupAttachment(RootComponent);

	ObstacleCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ObstacleCollision"));
	ObstacleCollision->SetupAttachment(RootComponent);
	
}

void AFloor::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Level Up"));
	AProceduralRunnerCharacter* Player = Cast<AProceduralRunnerCharacter>(OtherActor);
	if(Player)
	{
		if(Player->spawnyes == true){
			Player->spawnyes = false;
			if(ts){
					
					UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *OtherActor->GetName());
					ts->SpawnTile();
			}
			Player->moveSpeed += 1;
			Player->AddScore(1);
		}
	}
}

void AFloor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	AProceduralRunnerCharacter* Player = Cast<AProceduralRunnerCharacter>(OtherActor);
	if(Player)
	{
		Player->spawnyes = true;
		UE_LOG(LogTemp, Warning, TEXT("Exited"));
	}
}

 void AFloor::OnObstacleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
 {
 	UE_LOG(LogTemp, Warning, TEXT("Hit Obstacle"));
 	AProceduralRunnerCharacter* Player = Cast<AProceduralRunnerCharacter>(OtherActor);
 	if(Player)
 	{
 		UE_LOG(LogTemp, Warning, TEXT("LOOSE"));
 	}
 }


// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();

	EndCollision->OnComponentBeginOverlap.AddDynamic(this,&AFloor::OnBoxOverlap);
	EndCollision->OnComponentEndOverlap.AddDynamic(this,&AFloor::OnBoxEndOverlap);
	ObstacleCollision->OnComponentBeginOverlap.AddDynamic(this,&AFloor::OnObstacleOverlap);
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

