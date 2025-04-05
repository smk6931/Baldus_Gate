// Copyright Epic Games, Inc. All Rights Reserved.

#include "E_BaldusGateGameMode.h"
#include "UObject/ConstructorHelpers.h"

AE_BaldusGateGameMode::AE_BaldusGateGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
