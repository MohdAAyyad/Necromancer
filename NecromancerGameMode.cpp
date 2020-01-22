// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "NecromancerGameMode.h"
#include "NecromancerCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANecromancerGameMode::ANecromancerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
