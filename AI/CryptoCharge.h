// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyProjectile.h"
#include "Components/BoxComponent.h"
#include "CryptoCharge.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API ACryptoCharge : public AEnemyProjectile
{
	GENERATED_BODY()

		ACryptoCharge();

protected:
	void OnOverlap(UPrimitiveComponent* overlappedComponent_,
		AActor* otherActor_,
		UPrimitiveComponent* otherComp_,
		int32 otherBodyIndex_,
		bool bFromSweep_,
		const FHitResult &sweepResult_)override;

	UPROPERTY(EditAnywhere, Category = "Charge")
		float chargeLossDueToWall; //How much damage is lost when you hit a wall
};
