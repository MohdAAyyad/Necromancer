// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/AutoDialogue.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Merkvill.h"
#include "EnemyManager.h"
#include "MerkvillDialogue.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API AMerkvillDialogue : public AAutoDialogue
{
	GENERATED_BODY()
	
protected:
	AMerkvillDialogue();
	UPROPERTY(EditAnywhere, Category = Merkvill)
		UParticleSystemComponent* merkvillEntranceParticles;
	UPROPERTY(EditAnywhere, Category = Merkvill)
		AEnemyManager* enemyManagerRef;
	UPROPERTY(EditAnywhere, Category = Merkvill)
		TSubclassOf<AMerkvill> merkvillBP;
	UPROPERTY(EditAnywhere, Category = "Music")
		int switchMusicIndex;

	AMerkvill* merkvillRef;

	virtual void BeginPlay() override;
	void GetNextLineAndName(FString& name_, FString& line_) override;

		virtual void OverlapWithPlayer
		(UPrimitiveComponent* overlappedComponent_,
			AActor* otherActor_,
			UPrimitiveComponent* otherComp_,
			int32 otherBodyIndex_,
			bool bFromSweep_,
			const FHitResult &sweepResult_);

		UPROPERTY(EditAnywhere, Category = Shield)
			AMerkvillShield* shieldRef;

public:
	void ResetNPCDialogue() override;
};
