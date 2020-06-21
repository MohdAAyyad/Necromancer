// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DestructibleProp.h"
#include "../NPC/QuestNPC.h"
#include "QuestDestructibleProp.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API AQuestDestructibleProp : public ADestructibleProp
{
	GENERATED_BODY()
public:
	AQuestDestructibleProp();
	void PropTakeDamage(float damage_);
protected:
	UPROPERTY(EditAnywhere, Category="Prisoner")
		AQuestNPC* questNPC;
	UPROPERTY(EditAnywhere, Category = "Quest")
		FString questName;
	UPROPERTY(EditAnywhere, Category = "Quest")
		FString questDescription;

	UPROPERTY(EditAnywhere, Category = "Quest Auto Dialogue")
		class AQuestAutoDialogue* questAutoDialogue;
};
