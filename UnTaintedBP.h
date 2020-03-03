// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BloodPool.h"
#include "UnTaintedBP.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API AUnTaintedBP : public ABloodPool
{
	GENERATED_BODY()

protected:
		AUnTaintedBP();
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		virtual void Interact(float& value_, bool bp_) override; //If BP is true, value is updated with the pool's BP, and if false, with its HP

		float bloodPoints; //The amount of BPs this pool gives
		float healthPoints; //The amount of HPs this pool gives


	
};
