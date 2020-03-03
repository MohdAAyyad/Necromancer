// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AIAnimeInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API UAIAnimeInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

protected:
	APawn* owner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bZombify;

public:
	virtual void NextAttack() PURE_VIRTUAL(UAIAnimeInstanceBase::NextAttack, ;); //Overriden by children to determine what the next attack is
	virtual void HasSeenPlayer() PURE_VIRTUAL(UAIAnimeInstanceBase::HasSeenPlayer, ;); //Determine what animations play when you see player
	virtual void NextSpecial() PURE_VIRTUAL(UAIAnimeInstanceBase::NextSpecial, ;); //Every enemy has a special animation. Determine what the next one is
	virtual void NextCast() PURE_VIRTUAL(UAIAnimeInstanceBase::NextCast, ;); //Every enemy has a special animation. Determine what the next one is
	virtual void Reload() PURE_VIRTUAL(UAIAnimeInstanceBase::Reload, ;); //Every enemy has a reload animation.
	virtual void Death() PURE_VIRTUAL(UAIAnimeInstanceBase::Death, ;); //Every enemy needs to die


	UFUNCTION(BlueprintCallable)
		virtual void SetHit() PURE_VIRTUAL(UAIAnimeInstanceBase::SetHit, ;);
	UFUNCTION(BlueprintCallable)
		virtual void ResetAttack() PURE_VIRTUAL(UAIAnimeInstanceBase::ResetAttack, ;);
	UFUNCTION(BlueprintCallable)
		virtual void ResetCast() PURE_VIRTUAL(UAIAnimeInstanceBase::ResetCast, ;);
	UFUNCTION(BlueprintCallable)
		virtual void ResetSpecial() PURE_VIRTUAL(UAIAnimeInstanceBase::ResetSpecial, ;);
	UFUNCTION(BlueprintCallable)
		virtual void ResetReload() PURE_VIRTUAL(UAIAnimeInstanceBase::ResetReload, ;);
	UFUNCTION(BlueprintCallable)
		virtual void ResetHit() PURE_VIRTUAL(UAIAnimeInstanceBase::ResetHit, ;);
	UFUNCTION(BlueprintCallable)
		virtual void SpawnBP() PURE_VIRTUAL(UAIAnimeInstanceBase::SpawnBP, ;);

	UFUNCTION(BlueprintCallable)
		virtual void SetZombify() PURE_VIRTUAL(UAIAnimeInstanceBase::SetHit, ;);
	UFUNCTION(BlueprintCallable)
		virtual void ResetZombify() PURE_VIRTUAL(UAIAnimeInstanceBase::ResetAttack, ;);
};
