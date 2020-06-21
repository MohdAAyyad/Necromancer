// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "NecromancerGameMode.h"
#include "NecromancerCharacter.h"
#include "Quest/Checkpoint.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "NecromancerCharacter.h"
#include "Audio/MusicManager.h"
#include "SpellsInventory.h"
#include "QuestManager.h"
#include "EXPManager.h"
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
//	ANecromancerCharacter* playerPtr = Cast<ANecromancerCharacter>(GetWorld()->SpawnActor<AActor>(player, currentCheckpoint, FRotator::ZeroRotator));
	//if(playerPtr)
	//	GetWorld()->GetFirstPlayerController()->Possess(playerPtr);

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
	currentCheckpoint = FVector::ZeroVector;//If it's zero, the player's position will not be set to a checkpoint i.e. will spawn at the player start correctly
	currentLevelName = FName(TEXT("Tutorial")); //CurrentLevelName will change when the player reaches the end of each level through colliding with the end object
	SpellsInventory::GetInstance()->ResetInventory();
	EXPManager::GetInstance()->ResetEXP();
	QuestManager::GetInstance()->ResetQuest();
	RespawnPlayer();
}

void ANecromancerGameMode::SwitchLevel()
{
	currentCheckpoint = FVector::ZeroVector; //If it's zero, the player's position will not be set to a checkpoint i.e. will spawn at the player start correctly
	UGameplayStatics::OpenLevel(GetWorld(), currentLevelName, true);
}

void ANecromancerGameMode::FadeOutMusic()
{
	MusicManager::GetInstance()->FadeOutCurrentAudio();
}

void ANecromancerGameMode::DeathMusic()
{
	MusicManager::GetInstance()->SwtichMusic(8);
}

void ANecromancerGameMode::BackToMainMenu()
{
	currentLevelName = FName(TEXT("MainMenu"));
	RespawnPlayer();
}