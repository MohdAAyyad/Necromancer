// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Spells/Blood/BloodExplosion.h"
#include "NecromancerCharacter.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	enemyBT = CreateDefaultSubobject<UBehaviorTree>(TEXT("BehaviorTree"));
	sense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Sense"));
	bloodPoonSpawnLoc = CreateDefaultSubobject<USceneComponent>(TEXT("BloodpoolSummonLoc"));
	bloodPoonSpawnLoc->SetupAttachment(RootComponent);

	sense->SetPeripheralVisionAngle(60.0f);
	sense->bOnlySensePlayers = true;
	aiController = nullptr;
	currentStatusEffect = EStatusEffects::NONE;
	statusEffectDuration = 0.0f;
	enemyForZombie = nullptr;
	zombifyDuration = 30.0f;
	player = nullptr;
	enemyForZombie = nullptr;
	permenantTarget = nullptr;
	distractingZombie = nullptr;
	nextPatrolStopIndex = 0;
	prevPatrolStopIndex = -1;

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	aiController = Cast<AEnemyAIController>(GetController());
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBase::OnSeePlayer(APawn* pawn_)
{
	if (aiController)
	{
		if (!bDead || bZombie)
		{
			aiController->SetSeenTarget(pawn_);
		}
	}
}

void AEnemyBase::SpawnBloodPool()
{
	if (!bZombie)
	{
		FVector spawnLoc = GetMesh()->GetBoneLocation("Hips", EBoneSpaces::WorldSpace);
		spawnLoc.Z += 5000.0f; //The blood pool will always spawn on top of the first object it finds. Sometimes the enemy dies on a very low ground that when they die, the blood pool is spawned below the ground causing it to be deleted

		if (!bTypeOfBPToSpawn) //Untainted false, tainted true
		{
			if (bloodPools[0])
			{
				//Summon blood pool starting at the torso
				//Code should run after death animation is run
				GetWorld()->SpawnActor<ABloodPool>(bloodPools[0], spawnLoc, GetActorRotation());
			}
		}
		else
		{
			if (bloodPools[1])
			{
				//Summon blood pool starting at the torso
				//Code should run after death animation is run
				GetWorld()->SpawnActor<ABloodPool>(bloodPools[1], spawnLoc, GetActorRotation());
			}
		}
	}
	else
	{
		//When the zombie dies, destroy the body
		bZombie = false;
		AEnemyBase* en = Cast<AEnemyBase>(permenantTarget);
		if (en)
			en->distactingZombieIsDead();
		Destroy();
	}
}

float AEnemyBase::GetDistanceToPlayer()
{
	if (aiController)
		return ((aiController->GetPlayerLocation() - GetActorLocation()).Size());
	else
		return 10000.0f; //
}

void AEnemyBase::ApplyStatusEffect()
{
	switch (currentStatusEffect)
	{
	case POISONED:
		TakeSpellDamage(10.0f, EStatusEffects::NONE, 0.0f);
		UE_LOG(LogTemp, Warning, TEXT("hp - 10 = %f"), hp);
		//TODO
		//Add an indication that shows the status effect damage
		//TODO

		if (statusEffectDuration > 0.0f) //If the status effect is still active, call this function recursivly until the timer runs out
		{
			GetWorld()->GetTimerManager().SetTimer(statusEffectTimerHandle, this, &AEnemyBase::ApplyStatusEffect, 1.0f, false);
			statusEffectDuration--;
		}
		else
		{
			currentStatusEffect = EStatusEffects::NONE;
		}
		break;
	default:
		break;
	}
}

void AEnemyBase::Explode()
{
	if (explosion)
	{
		bZombie = false; //In case of a chain reaction, don't do a recursive call
		GetWorld()->SpawnActor<ABloodExplosion>(explosion, GetMesh()->GetComponentLocation(), FRotator::ZeroRotator);
		GetWorld()->GetTimerManager().SetTimer(statusEffectTimerHandle, this, &AEnemyBase::DestroyBody, 0.3f, false);
	}
}

void AEnemyBase::DelayedExplosion()
{
	GetWorld()->GetTimerManager().SetTimer(statusEffectTimerHandle, this, &AEnemyBase::Explode, 0.3f, false);
}

void AEnemyBase::AddEXP()
{
	EXPManager::GetInstance()->UpdateCurrentEXP(exp);
	hasAddedEXP = true;
}

void AEnemyBase::WhoToLookFor(APawn* pawn_)
{
	if (!distractingZombie) //If we do not have a reference to a distracting zombie, then operate normally
	{
		if (!player && !bZombie) //If we have yet to see the player, and we're not a zombie, make sure you look for the player
		{
			//This is to prevent the enemy from going after other enemies when it's not a zombie
			player = Cast<ANecromancerCharacter>(pawn_);
			if (player)
			{
				AEnemyBase::OnSeePlayer(player);
			}
		}
		else if (bZombie) //Otherwise look for other enemies
		{
			//UE_LOG(LogTemp, Warning, TEXT("Zombie seen this %s"), *pawn_->GetName());
			if (pawn_ != player)
			{
				enemyForZombie = Cast<AEnemyBase>(pawn_);
				if (enemyForZombie)
				{
					//UE_LOG(LogTemp,Warning,TEXT("Zombie has seen enemy"))
					if (!enemyForZombie->bZombie && !enemyForZombie->IsDead()) //Make sure you don't fight a fellow zombie
					{
						//UE_LOG(LogTemp, Warning, TEXT("Enemy is not zombie"))
						AEnemyBase::OnSeePlayer(pawn_);
					}
				}
			}
		}
		else if (player)
		{
			AEnemyBase::OnSeePlayer(player);
		}
	}
}

void AEnemyBase::Patrol()
{
	//Assumption: Enemy has not seen player yet. Enemy moves within a certain radius until player is seen

	if (patrolStops.Num() > 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Distance to %d = %f"), nextPatrolStopIndex, (moveLoc - GetActorLocation()).Size());
		if (nextPatrolStopIndex != prevPatrolStopIndex) //Next point has already been decided, move towards it
		{
			if(patrolStops[nextPatrolStopIndex])
				moveLoc = patrolStops[nextPatrolStopIndex]->GetActorLocation();
			prevPatrolStopIndex = nextPatrolStopIndex;

			if (aiController)
				aiController->SetNewLocation(moveLoc);
		}
		else if ((moveLoc - GetActorLocation()).Size() <= 120.0f) //Are we there yet?
		{
			//UE_LOG(LogTemp, Warning, TEXT("Reached Patrol Stop"));
			nextPatrolStopIndex++;
			if (nextPatrolStopIndex >= patrolStops.Num()) //If we've moved through all points, reset
			{
				nextPatrolStopIndex = 0;
				prevPatrolStopIndex = -1;
			}
		}

	}

}

void AEnemyBase::TakeSpellDamageFromZombie(APawn* zombie_, float damage_)
{
	//Update the distracting zombie ptr
	distractingZombie = zombie_;	
	permenantTarget = distractingZombie;
	if (aiController) //Update the controller's target
		aiController->SetSeenTarget(zombie_);
	TakeSpellDamage(damage_);
}

void AEnemyBase::distactingZombieIsDead() //Called by the distracting zombie to make sure the enemy attacks the player again
{
	UE_LOG(LogTemp, Warning, TEXT("Distracting Zombie Is Dead has been called"));
	distractingZombie = nullptr;
	permenantTarget = player;
	if (aiController)
		aiController->SetSeenTarget(player);
}