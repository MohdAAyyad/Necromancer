// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AimProjectile.h"
#include "BloodShot.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API ABloodShot : public AAimProjectile
{
	GENERATED_BODY()

		ABloodShot();
		void OnOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult &SweepResult) override;
};
