// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AimProjectile.h"
#include "BloodMiasma.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API ABloodMiasma : public AAimProjectile
{
	GENERATED_BODY()
		ABloodMiasma();
		void OnOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult &SweepResult) override;
	
};
