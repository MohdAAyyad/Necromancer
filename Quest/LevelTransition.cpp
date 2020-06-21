// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransition.h"
#include "../NecromancerCharacter.h"
#include "../NecromancerGameMode.h"

// Sets default values
ALevelTransition::ALevelTransition()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	box->SetGenerateOverlapEvents(true);
	box->SetCollisionProfileName("OverlapAllDynamic");
	box->SetupAttachment(root);

	particles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles"));
	particles->SetupAttachment(root);
	timeToCompleteTransition = 1.0f;
}

// Called when the game starts or when spawned
void ALevelTransition::BeginPlay()
{
	Super::BeginPlay();
	if (box)
		box->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransition::OnOverlap);
	gameMode = Cast<ANecromancerGameMode>(GetWorld()->GetAuthGameMode());
}

void ALevelTransition::OnOverlap(UPrimitiveComponent* overlappedComponent_,
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
			if (gameMode)
			{
				gameMode->SetLevelName(nextLevel);

				ALevelSequenceActor* sequenceActor;
				sequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), sequence, FMovieSceneSequencePlaybackSettings(), sequenceActor);

				if (sequencePlayer)
					sequencePlayer->Play();
				GetWorld()->GetTimerManager().SetTimer(timeToCompleteTransitionHandle, this, &ALevelTransition::CompleteTransition, timeToCompleteTransition, false);
			}
		}
	}
}

void ALevelTransition::CompleteTransition()
{
	if(gameMode)
	{
		gameMode->SwitchLevel();
	}
}
