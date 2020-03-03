// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BloodPool.h"
#include "TaintedBP.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API ATaintedBP : public ABloodPool
{
	GENERATED_BODY()
		ATaintedBP();
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
};
