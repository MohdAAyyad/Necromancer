// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AimProjectile.h"
#include "BloodRocket.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API ABloodRocket : public AAimProjectile
{
	GENERATED_BODY()

protected:
	ABloodRocket();
	UPROPERTY(EditAnywhere, Category = "Explosion")
		float explosionRadius;
	UPROPERTY(EditAnywhere, Category = "Explosion")
		float explosionDamage;
	UPROPERTY(EditAnywhere, Category = "Explosion")
		USphereComponent* explosionSphere;

	void BeginPlay() override;

	void OnOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult) override;
	
	UFUNCTION()
	void OnExplosionOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult);


	FTimerHandle destroyTimeHandle;
	float destroyDelay;
	virtual void DestroyProjectile();


};
