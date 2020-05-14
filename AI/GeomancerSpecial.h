// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyProjectile.h"
#include "TimerManager.h"
#include "GeomancerSpecial.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API AGeomancerSpecial : public AEnemyProjectile
{
	GENERATED_BODY()

protected:
	AGeomancerSpecial();
	void BeginPlay() override;
		UPROPERTY(EditAnywhere, Category = "Second Particles")
			UParticleSystemComponent* secondParticles; //The special attack gives an indicator for the player before attacking
		UPROPERTY(EditAnywhere)
			float timeToEnableCollision;
		FTimerHandle enableCollisiontimeHandle;

		UPROPERTY(EditAnywhere)
			float timeToEnableSecondParticle;
		FTimerHandle enableSecondParticletimeHandle;
		void EnableCollision();
		void EnableSecondCollision();

		void OnOverlap(UPrimitiveComponent* overlappedComponent_,
			AActor* otherActor_,
			UPrimitiveComponent* otherComp_,
			int32 otherBodyIndex_,
			bool bFromSweep_,
			const FHitResult &sweepResult_)override;
	
};
