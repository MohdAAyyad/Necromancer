// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SummonAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API USummonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
		USummonAnimInstance();
	UPROPERTY(BlueprintReadonly)
		float forwardSpeed;
	UPROPERTY(BlueprintReadonly)
		bool bAttack;
	UPROPERTY(BlueprintReadonly)
		bool bReload;
	UPROPERTY(BlueprintReadonly)
		bool bHit;
	UPROPERTY(BlueprintReadonly)
		bool bDead;
	UPROPERTY(BlueprintReadonly)
		bool bActive; //Becomes true after the "summoning" animation finishes

	class ASummonBase* summonOwner;


	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
		void UpdateAnimationProperties();


	UFUNCTION(BlueprintCallable)
		void ActivateSummon(); //Toggles true for the instance and the summon

	UFUNCTION(BlueprintCallable)
		void EnableHitBox();
	UFUNCTION(BlueprintCallable)
		void DisableHitBox();

	UFUNCTION(BlueprintCallable)
		void EndHit();

public:
	UFUNCTION(BlueprintCallable)
		void NextAttack();
	UFUNCTION(BlueprintCallable)
		void ResetReload();

	UFUNCTION(BlueprintCallable)
		void Death();
};
