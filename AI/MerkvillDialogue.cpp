// Fill out your copyright notice in the Description page of Project Settings.


#include "MerkvillDialogue.h"
#include "../NecromancerCharacter.h"
#include "../Audio/MusicManager.h"

AMerkvillDialogue::AMerkvillDialogue():AAutoDialogue()
{
	merkvillEntranceParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MerkvillEntranceComponent"));
	merkvillEntranceParticles->SetupAttachment(RootComponent);
	merkvillRef = nullptr;
}
void AMerkvillDialogue::BeginPlay()
{
	Super::BeginPlay();
	if (dialogueBox)
		dialogueBox->OnComponentBeginOverlap.AddDynamic(this, &AMerkvillDialogue::OverlapWithPlayer);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (merkvillEntranceParticles)
		merkvillEntranceParticles->DeactivateSystem();
}

void AMerkvillDialogue::OverlapWithPlayer
(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ != nullptr && otherActor_ != this && otherComp_ != nullptr)
	{
		ANecromancerCharacter* player = Cast<ANecromancerCharacter>(otherActor_); //Check if you collide with the player
		FRotator rot = FRotator::ZeroRotator;
		if (player)
		{
			MusicManager::GetInstance()->SwtichMusic(5);
			player->InitiateAutoDialogue(); //Initiate the conversation
			rot = (player->GetActorLocation() - merkvillEntranceParticles->GetComponentLocation()).Rotation();
		}

		if (merkvillEntranceParticles)
			merkvillEntranceParticles->ActivateSystem(true);

		FVector spawnLoc = merkvillEntranceParticles->GetComponentLocation();
		spawnLoc.Z += 400.0f;
		if(!merkvillRef)
			merkvillRef = GetWorld()->SpawnActor<AMerkvill>(merkvillBP, spawnLoc, rot);
		
		if (merkvillRef)
			if (shieldRef)
				merkvillRef->SetShieldRef(shieldRef);
	}
}
void AMerkvillDialogue::GetNextLineAndName(FString& name_, FString& line_)
{
	Super::GetNextLineAndName(name_, line_);
	if (lineIndex == switchMusicIndex)
	{
		MusicManager::GetInstance()->SwtichMusic(6);
	}
}

void AMerkvillDialogue::ResetNPCDialogue()
{
	lineIndex = 0;
	if (merkvillRef)
	{
		merkvillRef->StartFight();
		merkvillRef->SetEnemyManagerRef(enemyManagerRef);
		enemyManagerRef->AddEnemy(merkvillRef);
	}
	Destroy();
}