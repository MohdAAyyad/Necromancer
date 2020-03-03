// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AimProjectile.h"
#include "BloodExplosion.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API ABloodExplosion : public AAimProjectile
{
	GENERATED_BODY()
		ABloodExplosion();
	
	float explosionRadius;


	void OnOverlap(UPrimitiveComponent* overlappedComponent_,
		AActor* otherActor_,
		UPrimitiveComponent* otherComp_,
		int32 otherBodyIndex_,
		bool bFromSweep_,
		const FHitResult &sweepResult_) override;
	
};
