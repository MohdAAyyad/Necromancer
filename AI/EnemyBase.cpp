// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Spells/Blood/BloodExplosion.h"

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
			//GLog->Log("Saw player");
			aiController->SetSeenTarget(pawn_);
		}
	}
}

void AEnemyBase::SpawnBloodPool()
{
	//Override in children
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
