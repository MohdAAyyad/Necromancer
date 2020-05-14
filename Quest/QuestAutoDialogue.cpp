// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestAutoDialogue.h"
#include "../NecromancerCharacter.h"
#include "../QuestManager.h"

AQuestAutoDialogue::AQuestAutoDialogue()
{
	endQuestDescription = "Although Leah is on a quest to obtain higher powers in hopes\nof achieving her revenge, she does not allow herself\nto go down the path of a tyrant.\nDespite his promises of a special spell, Leah denied Kay-Hen\nhis request and freed the prisoner instead.";
}

void AQuestAutoDialogue::BeginPlay()
{
	Super::BeginPlay();
	if(dialogueBox)
		dialogueBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AQuestAutoDialogue::QuestPropEffect()
{
	if (dialogueBox)
		dialogueBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AQuestAutoDialogue::OverlapWithPlayer
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

		if (player)
			player->InitiateAutoDialogue(); //Initiate the conversation

		QuestManager::GetInstance()->CompleteQuest(questName, endQuestDescription); //End the quest

		for (int i = 0; i < questNPCs.Num(); i++)
		{
			questNPCs[i]->Destroy(); //Destory the NPCs related to the quest
		}
	}
}