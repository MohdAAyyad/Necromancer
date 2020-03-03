// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spells/Aim/BloodRocket.h"
#include "BloodTimeBomb.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API ABloodTimeBomb : public ABloodRocket
{
	GENERATED_BODY()

	ABloodTimeBomb();
	UPROPERTY(EditAnywhere)
	bool hasBeenAttached;

	void OnOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult) override;

	void Explode();
	void DestroyProjectile() override  { Super::DestroyProjectile(); } ;

	
};
