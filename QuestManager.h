// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class NECROMANCER_API QuestManager
{
public:
	~QuestManager();
	QuestManager(const QuestManager&) = delete;
	QuestManager(QuestManager&&) = delete;
	QuestManager& operator=(const QuestManager&) = delete;
	QuestManager& operator=(QuestManager&&) = delete;

	static QuestManager* GetInstance();
	FString GetQuestName(int index_);
	FString GetQuestDescription(int index_);
	void SetActiveQuest(int index_);
	FString GetActiveQuestName();
	int GetActiveQuestIndex();
	int GetQuestIndex(FString name_);
	void AddQuest(FString name_, FString description_);
	void UpdateQuest(FString name_, FString description_);
	void CompleteQuest(int index_, FString description_);
	void CompleteQuest(FString name_, FString description_);
	bool IsQuestCompleted(int index_);
	bool IsQuestCompleted(FString name_);
	int GetQuestsCount();
	int GetCompletedQuestsCount(); //Used to know when a quest is completed
	void RegisterUICTRL(class UPlayerUIController* uictrl_);

	void ResetQuest(); //Called from new game

private:
	static TUniquePtr<QuestManager, TDefaultDelete<QuestManager>> instance;
	QuestManager();
	TArray<FString> questsNames;
	TArray<FString> questsDescriptions;
	TArray<int> completedQuestsIndexes;
	int activeQuestIndex;
	UPlayerUIController* uictrl;
	void FindNextInCompleteQuest(); //Used to change the active quest index automatically when a quest is done
	void RewardEXP(FString name_); //Call EXP manager and adds EXP to player
};
