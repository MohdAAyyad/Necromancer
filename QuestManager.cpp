// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestManager.h"
#include "HUD/PlayerUIController.h"
#include "EXPManager.h"

TUniquePtr<QuestManager, TDefaultDelete<QuestManager>> QuestManager::instance = nullptr;

QuestManager::QuestManager()
{
	//Only need 2 for the sake of the demo
	questsNames.Reserve(2); 
	questsDescriptions.Reserve(2);
	completedQuestsIndexes.Reserve(2);

	activeQuestIndex = 0;
	uictrl = nullptr;
}

QuestManager::~QuestManager()
{
}

void QuestManager::RegisterUICTRL(UPlayerUIController* uictrl_)
{
	uictrl = uictrl_;
	//if(!questsNames.Contains("No longer safe"))
		//AddQuest("No longer safe", "The Ravens had scouts disguised as factory workers.\nTheir last transmission indicated that \nsoldiers from the Steel Army were seen in the vicinity.");
}

void QuestManager::AddQuest(FString name_, FString description_)
{
	if (!questsNames.Contains(name_)) //Is this a new quest?
	{
		questsNames.Push(name_);
		questsDescriptions.Push(description_);
		if (uictrl)
			uictrl->PlayNewQuestPrompt();
	}
	else //This is not a new quest, so update it with the new description
	{
		int index = questsNames.IndexOfByKey(name_);
		questsDescriptions[index] += "\n\n----------------------------------------------\n\n";
		questsDescriptions[index] += description_;
		if (uictrl)
			uictrl->PlayUpdatedQuestPrompt();
	}
}

void QuestManager::UpdateQuest(FString name_, FString description_)
{
	//Find the quest index
	int index = questsNames.IndexOfByKey(name_);

	//Update its description
	if(index < questsDescriptions.Num() && index >=0)
		questsDescriptions[index] = description_;
}
QuestManager* QuestManager::GetInstance()
{
	if (instance.Get() == nullptr)
	{
		instance.Reset(new QuestManager);
	}

	return instance.Get();
}

FString QuestManager::GetQuestName(int index_)
{
	if(index_<questsNames.Num() && index_>=0)
		return questsNames[index_];
	return "";
}
FString QuestManager::GetQuestDescription(int index_)
{
	if (index_ < questsDescriptions.Num() && index_ >= 0)
		return questsDescriptions[index_];
	return "";
}
void QuestManager::SetActiveQuest(int index_)
{
	if(index_<questsNames.Num() && index_>=0 && !IsQuestCompleted(index_))
		activeQuestIndex = index_;
}

FString QuestManager::GetActiveQuestName()
{
	if(activeQuestIndex<questsNames.Num() && activeQuestIndex>=0)
		return questsNames[activeQuestIndex];

	return "";
}
int QuestManager::GetActiveQuestIndex()
{
	return activeQuestIndex;
}
void QuestManager::CompleteQuest(int index_, FString description_)
{
	completedQuestsIndexes.Push(index_);
	questsDescriptions[index_] += "\n\n----------------------------------------------\n\n";
	questsDescriptions[index_] += description_;
	if (uictrl)
		uictrl->PlayCompletedQuestPrompt();

	FindNextInCompleteQuest(); //Update the active quest index
}

void QuestManager::CompleteQuest(FString name_, FString description_)
{
	int index = questsNames.IndexOfByKey(name_);
	completedQuestsIndexes.Push(index);
	questsDescriptions[index] += "\n\n----------------------------------------------\n\n";
	questsDescriptions[index] += description_;
	if (uictrl)
		uictrl->PlayCompletedQuestPrompt();
	RewardEXP(name_);
	FindNextInCompleteQuest(); //Update the active quest index
}

bool QuestManager::IsQuestCompleted(int index_)
{
	return completedQuestsIndexes.Contains(index_);
}

bool QuestManager::IsQuestCompleted(FString name_)
{
	//Find the quest index
	int index = questsNames.IndexOfByKey(name_);

	return IsQuestCompleted(index);
}

int QuestManager::GetCompletedQuestsCount()
{
	return completedQuestsIndexes.Num();
}

int QuestManager::GetQuestsCount()
{
	return questsNames.Num();
}
int QuestManager::GetQuestIndex(FString name_)
{
	if (questsNames.Contains(name_))
	{
		return questsNames.IndexOfByKey(name_);
	}
	return -1;
}

void QuestManager::FindNextInCompleteQuest()
{
	for (int i = 0; i < questsNames.Num(); i++)
	{
		if (!IsQuestCompleted(i))
		{
			activeQuestIndex = i; //Find the first incomplete quest and make it the active quest
			break;
		}
	}

	if (uictrl)
		uictrl->SetShownQuest(activeQuestIndex); //Update the UI
}

void QuestManager::RewardEXP(FString name_)
{
	if (name_ == "No longer safe")
	{
		EXPManager::GetInstance()->UpdateCurrentEXP(2000);
	}
	else if (name_ == "A son's hope. A devil's advocate. ")
	{
		EXPManager::GetInstance()->UpdateCurrentEXP(1000);
	}
}

void QuestManager::ResetQuest()
{
	if(questsNames.Num()>0)
		questsNames.Empty();
	if(questsDescriptions.Num()>0)
		questsDescriptions.Empty();
	if(completedQuestsIndexes.Num()>0)
		completedQuestsIndexes.Empty();

	activeQuestIndex = 0;
}