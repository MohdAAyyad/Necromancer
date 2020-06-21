// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathVolume.h"
#include "NecromancerCharacter.h"


// Sets default values
ADeathVolume::ADeathVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = root;

	box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	box->SetCollisionProfileName("OverlapAll");
	box->SetupAttachment(root);
}

void ADeathVolume::BeginPlay()
{
	Super::BeginPlay();

	box->OnComponentBeginOverlap.AddDynamic(this, &ADeathVolume::OnPlayerOverlap);
}

void ADeathVolume::OnPlayerOverlap(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ != nullptr && otherActor_ != this && otherComp_ != nullptr)
	{
		ANecromancerCharacter* player = Cast<ANecromancerCharacter>(otherActor_);
		if (player)
		{
			player->PlayerTakeDamage(100000000.0f); //Kill the player
		}
	}
}

