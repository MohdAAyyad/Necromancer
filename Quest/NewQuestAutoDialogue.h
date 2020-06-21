// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/AutoDialogue.h"
#include "NewQuestAutoDialogue.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API ANewQuestAutoDialogue : public AAutoDialogue
{
	GENERATED_BODY()
protected:
	ANewQuestAutoDialogue();
	UPROPERTY(EditAnywhere, Category = "QuestIndex")
		int questIndex; //Which line exactly invokes the quest manager?

	UPROPERTY(EditAnywhere, Category = "Quest")
		FString questName;
	UPROPERTY(EditAnywhere, Category = "Quest")
		FString questDescription;
public:
	virtual void GetNextLineAndName(FString& name_, FString& line_) override;
};
