// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint.h"
#include "../NecromancerCharacter.h"
#include "../NecromancerGameMode.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = root;
	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	sphere->SetGenerateOverlapEvents(true);
	sphere->SetCollisionProfileName("OverlapAllDynamic");
	sphere->SetupAttachment(root);

	inactiveParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Inactive Particles"));
	inactiveParticles->SetupAttachment(root);

	activeParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Active Particles"));
	activeParticles->SetupAttachment(root);

	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	audioComponent->SetupAttachment(root);

	timeToDisableP1 = 0.5f;
	hpToRestore = 150.0f;
	bpToRestore = 150.0f;

}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	if (activeParticles)
		activeParticles->DeactivateSystem();

	if (sphere)
		sphere->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnOverlap);
	
	if (audioComponent)
		audioComponent->Sound = sound;
	
}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckpoint::OnOverlap(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ != nullptr && otherActor_ != this && overlappedComponent_ != nullptr)
	{
		//When you collide with the player, update the game mode's current checkpoint
		ANecromancerCharacter* player = Cast<ANecromancerCharacter>(otherActor_);

		if (player)
		{
			//Update the current checkpoint
			ANecromancerGameMode* gameMode = Cast<ANecromancerGameMode>(GetWorld()->GetAuthGameMode());

			if (gameMode)
			{
				gameMode->SetNewCheckpoint(GetActorLocation());
				if (activeParticles) //Activate the activeParticles signaling that the checkpoint has been triggered
					activeParticles->ActivateSystem(true);
				GetWorld()->GetTimerManager().SetTimer(timerHandleToDisableP1, this, &ACheckpoint::DisableInactiveParticles, timeToDisableP1, false); //Disable the inactiveParticles
				sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//Play sound
				if (audioComponent)
					audioComponent->Play();
			}

			//Heal player
			player->HealthDueToCheckpoint(hpToRestore, bpToRestore);
		}
	}
}

void ACheckpoint::DisableInactiveParticles()
{
	if (inactiveParticles)
		inactiveParticles->DeactivateSystem();
}

