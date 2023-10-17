// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProceduralRunnerGameMode.h"
#include "ProceduralRunnerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProceduralRunnerGameMode::AProceduralRunnerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
