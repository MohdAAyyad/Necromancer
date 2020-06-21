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
		class ANecromancerCharacter* necro;
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
			bool baimMode; //True: Player aims, false: player idle
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			bool banimAimShoot; //Becomes true when player shoots and false when animation ends

		UPROPERTY(EditAnywhere, BlueprintReadOnly)
			bool bHit; //True when the player gets hit

		UPROPERTY(EditAnywhere, BlueprintReadOnly)
			bool bDash;

		UPROPERTY(EditAnywhere, BlueprintReadOnly)
			bool bDead;


		//Spells
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
			bool buseSpell1;
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
			bool buseSpell2;
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
			bool buseBloodSpell1;
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
			bool buseBloodSpell2;

		//Wall
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
			bool bwall;


		UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
			void UpdateAnimationProperties();
		
		UFUNCTION(BlueprintCallable)
			void ResetSpellAnimation();

		UFUNCTION(BlueprintCallable)
			void ResetHit();
		UFUNCTION(BlueprintCallable)
			void ResetDash();
		UFUNCTION(BlueprintCallable)
			void MoveDash();
		UFUNCTION(BlueprintCallable)
			void ConjurSpell();
		UFUNCTION(BlueprintCallable)
			void SpawnWall();
		UFUNCTION(BlueprintCallable)
			void Death();


};
