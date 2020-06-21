// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.h"
#include "EnemyManager.generated.h"

//Handles zombification core functions
//Tells zombies which enemies to attack

UCLASS()
class NECROMANCER_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Enemies")
		TArray<AEnemyBase*> allEnemies; //Reference is removed when enemy dies
	TArray<AEnemyBase*> deadEnemies; //Filled with dead enemy references. When an enemy is turned into a zombie, the reference is removed
	TArray<AEnemyBase*> zombieEnemies; //Filled with zombie enemy references. We need it to have the zombie change targets after the first target is dead
public:	
	AEnemyBase* GetTargetForZombie();
	void EnemyDeath(AEnemyBase* ref_);
	void EnemyZombie(AEnemyBase* ref_);
	void RemoveZombie(AEnemyBase* ref_);
	void AddEnemy(AEnemyBase* ref_); //Used to add enemies at runtime
};
