// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "TimerManager.h"
#include "LevelTransition.generated.h"

UCLASS()
class NECROMANCER_API ALevelTransition : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelTransition();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Root")
		USceneComponent* root;
	UPROPERTY(EditAnywhere, Category = "Box")
		UBoxComponent* box;
	UPROPERTY(EditAnywhere, Category = "Level")
		FName nextLevel;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystemComponent* particles;

	UPROPERTY(EditAnywhere, Category = "Level Sequence")
		ULevelSequence* sequence;
	ULevelSequencePlayer* sequencePlayer;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* overlappedComponent_,
			AActor* otherActor_,
			UPrimitiveComponent* otherComp_,
			int32 otherBodyIndex_,
			bool bFromSweep_,
			const FHitResult &sweepResult_);

	FTimerHandle timeToCompleteTransitionHandle;
	float timeToCompleteTransition;
	void CompleteTransition();

	class ANecromancerGameMode* gameMode;

};
