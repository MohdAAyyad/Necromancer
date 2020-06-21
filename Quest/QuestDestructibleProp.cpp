// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestDestructibleProp.h"
#include "../QuestManager.h"
#include "QuestAutoDialogue.h"

AQuestDestructibleProp::AQuestDestructibleProp():ADestructibleProp()
{
	questNPC = nullptr;
}

void AQuestDestructibleProp::PropTakeDamage(float damage_)
{
	if (QuestManager::GetInstance()->GetQuestIndex(questName) != -1) //Don't get destroyed if we haven't got the quest yet
	{
		Super::PropTakeDamage(damage_);
		if (questNPC)
			questNPC->QuestUpdate();
		
		if (questAutoDialogue)
			questAutoDialogue->QuestPropEffect(); //Enable the auto dialogues effect
	}
}