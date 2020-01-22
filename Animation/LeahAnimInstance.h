// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LeahAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API ULeahAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
		APawn* player;
		ULeahAnimInstance();

public:
		//Non Aim movement
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
			float forwardSpeed;

		//Aim mode Movement
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
			float aimForwardSpeed;
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
			float aimRightSpeed;
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
			float aimLeftSpeed;
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
			float aimBackSpeed;
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
			bool baimMode;

		UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
			void UpdateAnimationProperties();

};
