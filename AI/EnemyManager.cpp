// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	if (allEnemies.Num() > 0) //Give your enemies a reference to you
	{
		for (int i = 0; i < allEnemies.Num(); i++)
		{
			if (allEnemies[i] != nullptr)
			{
				allEnemies[i]->SetEnemyManagerRef(this);
			}
		}
	}
}

AEnemyBase* AEnemyManager::GetTargetForZombie()
{
	if (allEnemies.Num() > 0)
	{
		int choice = FMath::RandRange(0, allEnemies.Num() - 1);
		return allEnemies[choice];
	}
	return nullptr;
}
void AEnemyManager::EnemyDeath(AEnemyBase* ref_)
{
	//Remove the enemy reference from all enemies and move it to dead enemies
	if (allEnemies.Contains(ref_))
	{
		allEnemies.Remove(ref_);
	}
	deadEnemies.Push(ref_);

	//Tell the zombies to check if they need a new target in case this dead enemy is their original target
	if (zombieEnemies.Num() > 0)
	{
		for (int i = 0; i < zombieEnemies.Num(); i++)
		{
			if (zombieEnemies[i] != nullptr)
			{
				zombieEnemies[i]->ZombieTargetIsDead(ref_); 
			}
		}
	}
}

void AEnemyManager::EnemyZombie(AEnemyBase* ref_)
{
	//Remove the reference from the dead enemies

	if (deadEnemies.Contains(ref_))
	{
		deadEnemies.Remove(ref_);
	}
	zombieEnemies.Push(ref_);
}

void AEnemyManager::RemoveZombie(AEnemyBase* ref_)
{
	if(zombieEnemies.Contains(ref_))
		zombieEnemies.Remove(ref_);
}

void AEnemyManager::AddEnemy(AEnemyBase* ref_)
{
	allEnemies.Push(ref_);
}
