// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/NPC.h"
#include "AutoDialogue.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API AAutoDialogue : public ANPC
{
	GENERATED_BODY()
	
protected:

	void BeginPlay() override;
	UFUNCTION()
		virtual void OverlapWithPlayer
		(UPrimitiveComponent* overlappedComponent_,
			AActor* otherActor_,
			UPrimitiveComponent* otherComp_,
			int32 otherBodyIndex_,
			bool bFromSweep_,
			const FHitResult &sweepResult_);

public:
	void ResetNPCDialogue() override;
};
