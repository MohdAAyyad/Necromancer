// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIAnimeInstanceBase.h"
#include "MerkvillAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API UMerkvillAnimInstance : public UAIAnimeInstanceBase
{
	GENERATED_BODY()
	
protected:

	UMerkvillAnimInstance();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bAttack0; //Fireballs
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bAttack1; //Traps
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bAttack2; //Tornadoes
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bAttack3; //Explosion
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bDeath;

public:

	void NextAttack()override;//Overriden by children to determine what the next attack is
	void NextAttack1();
	void NextAttack2();
	void NextAttack3();

	UFUNCTION(BlueprintCallable)
		void FireBall();
	UFUNCTION(BlueprintCallable)
		void Meteor();
	UFUNCTION(BlueprintCallable)
		void Tornado();

	UFUNCTION(BlueprintCallable)
		void CompleteExplosion();

	void ResetAttack() override;
	UFUNCTION(BlueprintCallable)
		void ResetAttack1();
	UFUNCTION(BlueprintCallable)
		void ResetAttack2();
	UFUNCTION(BlueprintCallable)
		void ResetAttack3();

	void Death() override; //Every enemy needs to die
	void SpawnBP() override;

	//Reset functions reset the active boolean of each type
	void SetHit() override;
	void ResetHit() override;
	void Reload() override;
	void SetStun();
	void ResetStun();

protected:

	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
		void UpdateAnimationProperties();

	class AMerkvill* merkvill;

};
