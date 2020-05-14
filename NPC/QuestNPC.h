// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/NPC.h"
#include "../QuestManager.h"
#include "QuestNPC.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API AQuestNPC : public ANPC
{
	GENERATED_BODY()
public:
	AQuestNPC();
	virtual void QuestUpdate() {}; //Called by objects relevant to the quest itself //Should be overriden by children for desired effects
protected:
	UPROPERTY(EditAnywhere, Category = "QuestIndex")
		int questIndex; //Which line exactly invokes the quest manager?

	UPROPERTY(EditAnywhere, Category = "Quest")
		FString questName;
	UPROPERTY(EditAnywhere, Category = "Quest")
		FString questDescription;
	UPROPERTY(EditAnywhere, Category = "Quest")
		FString endQuestDescription;
public:
	 virtual void GetNextLineAndName(FString& name_, FString& line_) override;
};
