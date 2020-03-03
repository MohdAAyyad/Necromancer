// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIAnimeInstanceBase.h"
#include "Animation/AnimMontage.h"
#include "ArcheressAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API UArcheressAnimInstance : public UAIAnimeInstanceBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bAttack0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bCast0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bDeath;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bHit;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bAim;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bShoot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UAnimMontage* aimShootMontage;
public:
	void NextAttack()override;//Overriden by children to determine what the next attack is
	void Death() override; //Every enemy needs to die
	void SpawnBP() override;
	void NextCast() override;

	//Reset functions reset the active boolean of each type
	void ResetAttack() override;
	void ResetCast() override;
	void SetHit() override;
	void ResetHit() override;
	void SetZombify() override;
	void ResetZombify() override;

	void SetAim();

	bool IsMontagePlaying();

protected:

	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
		void UpdateAnimationProperties();

	UArcheressAnimInstance();

	UFUNCTION(BlueprintCallable, Category = "Cast")
		void CastSpell(); //Use Cast spell.

	class AArcheress* archeress;
	
};
