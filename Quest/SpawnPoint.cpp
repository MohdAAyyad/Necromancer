// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoint.h"
#include "NecromancerGameMode.h"
#include "Engine/World.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	ANecromancerGameMode* gameMode = Cast<ANecromancerGameMode>(GetWorld()->GetAuthGameMode());

	if (gameMode)
	{
		gameMode->SetNewCheckpoint(GetActorLocation());
	}
}

