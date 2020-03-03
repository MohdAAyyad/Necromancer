// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIAnimeInstanceBase.h"
#include "KnightAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API UKnightAnimInstance : public UAIAnimeInstanceBase
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
		void UpdateAnimationProperties();

	UKnightAnimInstance();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float forwardSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAttack0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCast0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSpecial0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bReload;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDeath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bStrafe;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bHit;



public:
	 void NextAttack()override;//Overriden by children to determine what the next attack is
	 void NextSpecial() override; //Every enemy has a special animation. Determine what the next one is
	 void Reload() override;//Every enemy has a reload animation.
	 void Death() override; //Every enemy needs to die
	 void SpawnBP() override;
	 void NextCast() override;
	 
	 void SetStrafe();
	 void ResetStrafe();

	 //Reset functions reset the active boolean of each type
	 void ResetAttack() override;
	 void ResetCast() override;
	 void ResetSpecial() override;
	 void ResetReload() override;
	 void SetHit() override;
	 void ResetHit() override;
	 void SetZombify() override;
	 void ResetZombify() override;


protected:

	UFUNCTION(BlueprintCallable, Category = "Enable Attack Hit Box")
		void EnableAttackHitBox();

	UFUNCTION(BlueprintCallable, Category = "Cast")
		void CastSpell(); //Use Cast spell. Knight only has one spell

	class AKnight* knight;



};
