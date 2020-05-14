// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/QuestNPC.h"
#include "../AI/EnemyBase.h"
#include "TimerManager.h"
#include "BehaviorTree/BehaviorTree.h"
#include "../AI/EnemyAIController.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "../Animation/NPC/PrisonerAnimInstance.h"
#include "../Quest/QuestAutoDialogue.h"
#include "PrisonerQuestNPC.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API APrisonerQuestNPC : public AQuestNPC
{
	GENERATED_BODY()
		APrisonerQuestNPC();
public:
	void Zombify();
	void ActivateZombie();
	void QuestUpdate() override;

	UPROPERTY(EditAnywhere, Category = Behavior)
		UBehaviorTree* prisonerBT;

	void GetNextLineAndName(FString& name_, FString& line_) override;

protected:
	 void BeginPlay() override;
	 void Death();
	 void EndZombify();
	 bool bDead;
	 UPROPERTY(EditAnywhere, Category = "Quest destination")
		 AActor* destination; //Moves to this destination after being zombified

	 UPROPERTY(EditAnywhere, Category = "Damage")
		 UBoxComponent* damageBox;


	 UPROPERTY(EditAnywhere, Category = "Destination")
		 UParticleSystemComponent* destinationParticles;
	 UPROPERTY(EditAnywhere, Category = "Quest Auto Dialogue")
		 AQuestAutoDialogue* autoDialogue;


	 UFUNCTION()
		 void DestinationOverlap
		 (UPrimitiveComponent* overlappedComponent_,
			 AActor* otherActor_,
			 UPrimitiveComponent* otherComp_,
			 int32 otherBodyIndex_,
			 bool bFromSweep_,
			 const FHitResult &sweepResult_);

	 void DestroySelf();

	 FTimerHandle zombifyTimerHandle;
	 float zombifyDuration;

	 AEnemyAIController* aiController;
	 UPrisonerAnimInstance* animInstance;
};
