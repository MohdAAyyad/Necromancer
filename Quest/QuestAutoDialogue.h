// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/AutoDialogue.h"
#include "AffectedByQuestProp.h"
#include "QuestAutoDialogue.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API AQuestAutoDialogue : public AAutoDialogue, public IAffectedByQuestProp
{
	GENERATED_BODY()

public:
	void QuestPropEffect() override;
	AQuestAutoDialogue();
protected:
	void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Quest")
		FString questName;
	UPROPERTY(EditAnywhere, Category = "Quest")
		FString questDescription;
	UPROPERTY(EditAnywhere, Category = "Quest")
		FString endQuestDescription;

	UPROPERTY(EditAnywhere, Category = "Quest")
		TArray<ANPC*> questNPCs; //The NPCs related to the quest should be destroyed
	UPROPERTY(EditAnywhere, Category = "Music")
		int musicIndex;

	void OverlapWithPlayer
	(UPrimitiveComponent* overlappedComponent_,
		AActor* otherActor_,
		UPrimitiveComponent* otherComp_,
		int32 otherBodyIndex_,
		bool bFromSweep_,
		const FHitResult &sweepResult_) override;

	void ResetNPCDialogue() override;
	
};
