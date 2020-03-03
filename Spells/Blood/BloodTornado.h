// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AimProjectile.h"
#include "Components/BoxComponent.h"
#include "BloodTornado.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API ABloodTornado : public AAimProjectile
{
	GENERATED_BODY()

	ABloodTornado();
	
	UPROPERTY(EditAnywhere, Category = Box)
		UBoxComponent* box;

	void BeginPlay() override;
	void OnOverlap(UPrimitiveComponent* overlappedComponent_,
		AActor* otherActor_,
		UPrimitiveComponent* otherComp_,
		int32 otherBodyIndex_,
		bool bFromSweep_,
		const FHitResult &sweepResult_) override;
	
};
