// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoDialogue.h"
#include "../NecromancerCharacter.h"
#include "Components/CapsuleComponent.h"

void AAutoDialogue::BeginPlay()
{
	Super::BeginPlay();
	if(dialogueBox)
		dialogueBox->OnComponentBeginOverlap.AddDynamic(this, &AAutoDialogue::OverlapWithPlayer);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAutoDialogue::OverlapWithPlayer (UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ != nullptr && otherActor_ != this && otherComp_ != nullptr)
	{
		ANecromancerCharacter* player = Cast<ANecromancerCharacter>(otherActor_); //Check if you collide with the player

		if (player)
			player->InitiateAutoDialogue(); //Initiate the conversation
	}
}

void AAutoDialogue::ResetNPCDialogue()
{
	Super::ResetNPCDialogue();
	Destroy(); //Purpose is done, destroy self
}