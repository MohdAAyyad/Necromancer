// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AimProjectile.h"
#include "EnemyProjectile.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API AEnemyProjectile : public AAimProjectile
{
	GENERATED_BODY()

public:
	AEnemyProjectile();
	void ChangeProfileName(FString profile_);

protected:

	float damage;

		void OnOverlap(UPrimitiveComponent* overlappedComponent_,
			AActor* otherActor_,
			UPrimitiveComponent* otherComp_,
			int32 otherBodyIndex_,
			bool bFromSweep_,
			const FHitResult &sweepResult_)override;

	    void BeginPlay() override;
		void BindSphere() override;
};
