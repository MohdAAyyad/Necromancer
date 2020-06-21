// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDetector.h"
#include "../NecromancerCharacter.h"

// Sets default values
APlayerDetector::APlayerDetector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	box->SetGenerateOverlapEvents(true);
	box->SetCollisionProfileName("AliveEnemyProjectile");
	box->SetupAttachment(RootComponent);
}

void APlayerDetector::BeginPlay()
{
	Super::BeginPlay();
	if (box)
		box->OnComponentBeginOverlap.AddDynamic(this, &APlayerDetector::PlayerOverlap);
}

void APlayerDetector::PlayerOverlap
(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ != nullptr && otherComp_ != nullptr && otherActor_ != this)
	{
		ANecromancerCharacter* player = Cast<ANecromancerCharacter>(otherActor_);
		if (player)
		{
			for (int i = 0; i < enemies.Num(); i++)
			{
				enemies[i]->PlayerHasBeenDetectedByDetector(player); //Take regular damage will make sure enemies update their target to the player
			}
			Destroy(); //Job done
		}
	}
}

