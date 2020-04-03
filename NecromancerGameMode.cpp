// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "NecromancerGameMode.h"
#include "NecromancerCharacter.h"
#include "Quest/Checkpoint.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

FVector ANecromancerGameMode::currentCheckpoint = FVector::ZeroVector;

ANecromancerGameMode::ANecromancerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ANecromancerGameMode::SetNewCheckpoint(FVector checkpointLoc_)
{
	currentCheckpoint = checkpointLoc_;
}

void ANecromancerGameMode::RespawnPlayer()
{
	//Reload current level
	UGameplayStatics::OpenLevel(GetWorld(), currentLevelName, true);
	//If there's a checkpoint, move the player to the checkpoint
	//CurrentCheckPoint is set to nullptr when we load new levels
	//if (currentCheckpoint)
//	{
	//	GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(currentCheckpoint->GetCheckpointLocation());
	//}
	//Otherwise, the player will spawn where the networkplayerstart is at inside the level
}

//Called when level transitioning
void ANecromancerGameMode::SetLevelName(FName level_)
{
	currentLevelName = level_;
}

FVector ANecromancerGameMode::GetStartingPosition()
{
	return currentCheckpoint;
}
