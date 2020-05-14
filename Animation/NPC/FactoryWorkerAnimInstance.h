// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FactoryWorkerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API UFactoryWorkerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
		
public:
	UFactoryWorkerAnimInstance();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bDead;

protected:

	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
		void UpdateAnimationProperties();
};
