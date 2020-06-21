// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIAnimeInstanceBase.h"
#include "RavenAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API URavenAnimInstance : public UAIAnimeInstanceBase
{
	GENERATED_BODY()
protected:
	URavenAnimInstance();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bAttack0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bDead;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bHit;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bSkeleton;

	class ARaven* raven;

	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
		void UpdateAnimationProperties();



public:
	void NextAttack()override;
	UFUNCTION(BlueprintCallable)
		void SpawnAimProj();
	void ResetAttack()override;
	void SetHit() override;
	void ResetHit() override;
	void SetZombify() override;
	void ResetZombify() override;
	void Death() override;

	UFUNCTION(BlueprintCallable)
		void SummonSkeleton();
	UFUNCTION(BlueprintCallable)
		void SpawnSkeleton();
	UFUNCTION(BlueprintCallable)
		void EndDeath();
};
