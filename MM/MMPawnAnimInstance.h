// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "MMPawnAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API UMMPawnAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
		UMMPawnAnimInstance();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bRise;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bEnemyRise;

	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
		void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAnimMontage* sitToStandMontage;
public:
		void SetRise(bool value_);
		void SetEnemyRise(bool value_);
};
