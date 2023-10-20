// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralRunner\Public\LowerBorder.h"

#include "Components/BoxComponent.h"
#include "ProceduralRunner/ProceduralRunnerCharacter.h"

// Sets default values
ALowerBorder::ALowerBorder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = SceneRoot;
	Border = CreateDefaultSubobject<UBoxComponent>(TEXT("Border"));
	Border->SetupAttachment(RootComponent);
}



void ALowerBorder::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	
	AProceduralRunnerCharacter* Player = Cast<AProceduralRunnerCharacter>(OtherActor);
	if(Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("BorderHIT!"))
		RestartGame();
	}
}

// Called when the game starts or when spawned
void ALowerBorder::BeginPlay()
{
	Super::BeginPlay();
	Border->OnComponentBeginOverlap.AddDynamic(this,&ALowerBorder::OnBoxOverlap);
}

// Called every frame
void ALowerBorder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

