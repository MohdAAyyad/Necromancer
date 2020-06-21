// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/AutoDialogue.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "TimerManager.h"
#include "MerkvillEndDialogue.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API AMerkvillEndDialogue : public AAutoDialogue
{
	GENERATED_BODY()

		AMerkvillEndDialogue();
	
protected:
		class AMerkvill* merkvill;

		UPROPERTY(EditAnywhere, Category = "End Particles")
			UParticleSystemComponent* explosionIndicatorParticles;
		UPROPERTY(EditAnywhere, Category = "End Particles")
			UParticleSystemComponent* explosionParticles;
		UPROPERTY(EditAnywhere, Category = "End Particles")
			int indicatorParticlesIndex;
		UPROPERTY(EditAnywhere, Category = "End Particles")
			int explosionParticlesIndex;
		UPROPERTY(EditAnywhere, Category = "Music")
			int switchMusicIndex;
		UPROPERTY(EditAnywhere, Category = "Level Sequence")
			ULevelSequence* sequence;
		ULevelSequencePlayer* sequencePlayer;

		void BeginPlay() override;

public:
		void GetNextLineAndName(FString& name_, FString& line_) override;
		void SetMerkvillRef(class AMerkvill* merkvill_);
		void ResetNPCDialogue() override;
};
