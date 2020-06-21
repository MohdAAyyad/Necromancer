// Fill out your copyright notice in the Description page of Project Settings.


#include "EndBattleMusic.h"
#include "../NecromancerCharacter.h"

// Sets default values
AEndBattleMusic::AEndBattleMusic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = root;
	box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	box->SetGenerateOverlapEvents(true);
	box->SetCollisionProfileName("AliveEnemyProjectile");
	box->SetupAttachment(root);
	playThisMusicNext = 2;
	bGetsDestroyedAfterSwitch = true;

}

// Called when the game starts or when spawned
void AEndBattleMusic::BeginPlay()
{
	Super::BeginPlay();
	if (box)
		box->OnComponentBeginOverlap.AddDynamic(this, &AEndBattleMusic::MusicOverlap);
}

void AEndBattleMusic::MusicOverlap
(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &SweepResult_)
{
	if (otherActor_ != nullptr && otherActor_ != this && otherComp_ != nullptr)
	{
		ANecromancerCharacter* player = Cast<ANecromancerCharacter>(otherActor_);

		if (player)
		{
			MusicManager::GetInstance()->SwtichMusic(playThisMusicNext);
			if(bGetsDestroyedAfterSwitch)
				Destroy();
		}
	}
}
