// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_SGameMode.h"
#include "TP_SCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATP_SGameMode::ATP_SGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
