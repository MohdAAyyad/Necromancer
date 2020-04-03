// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyProjectile.h"
#include "TimerManager.h"
#include "GeomancerSpikes.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API AGeomancerSpikes : public AEnemyProjectile
{
	GENERATED_BODY()
protected:
	AGeomancerSpikes();
	void BeginPlay() override;

		FTimerHandle enableCollisiontimeHandle; //Spikes should not have their collision working until the spikes are out

		UPROPERTY(EditAnywhere)
			float timeToEnableCollision;
		void EnableCollision();
	
	
};
