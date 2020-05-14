// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/QuestNPC.h"
#include "../Animation/NPC/FactoryWorkerAnimInstance.h"
#include "FactoryWorker.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API AFactoryWorker : public AQuestNPC
{
	GENERATED_BODY()
		AFactoryWorker();
protected:

		UFactoryWorkerAnimInstance* animInstance;
		void BeginPlay() override;
		void ResetNPCDialogue() override;
		void GetNextLineAndName(FString& name_, FString& line_) override;
};
