// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PrisonerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API UPrisonerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
		UPrisonerAnimInstance();

		void SetDead();
		void SetZombie();
		void SetThanks(bool value_);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bZombie;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bThanks;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float forwardSpeed;
		
UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
		void UpdateAnimationProperties();

UFUNCTION(BlueprintCallable, Category = "ActivateZombie")
		void ActivateZombie();

class APrisonerQuestNPC* prisoner;
};
