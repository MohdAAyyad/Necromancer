// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "NecromancerGameMode.h"
#include "NecromancerCharacter.h"
#include "Quest/Checkpoint.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "NecromancerCharacter.h"
#include "HUD/PlayerHUD.h"

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

void ANecromancerGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ANecromancerGameMode::SetNewCheckpoint(FVector checkpointLoc_)
{
	currentCheckpoint = checkpointLoc_;
}

void ANecromancerGameMode::RespawnPlayer()
{
	//Reload current level
	UGameplayStatics::OpenLevel(GetWorld(), currentLevelName, true);
	ANecromancerCharacter* playerPtr = Cast<ANecromancerCharacter>(GetWorld()->SpawnActor<AActor>(player, currentCheckpoint, FRotator::ZeroRotator));
	if(playerPtr)
		GetWorld()->GetFirstPlayerController()->Possess(playerPtr);

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

void ANecromancerGameMode::NewGame()
{
	currentLevelName = FName(TEXT("MA1-1_Intro"));
	RespawnPlayer();
}