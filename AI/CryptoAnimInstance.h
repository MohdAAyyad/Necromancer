// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIAnimeInstanceBase.h"
#include "CryptoAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API UCryptoAnimInstance : public UAIAnimeInstanceBase
{
	GENERATED_BODY()
protected:
		UCryptoAnimInstance();

		UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
		void UpdateAnimationProperties();

	class ACrypto* crypto;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bCast0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bSpecial0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bReload;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bDeath;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bHit;

public:
	void Reload() override;
	void Death() override; 
	void SpawnBP() override;
	void NextCast() override;
	void NextSpecial() override; 
	void ResetCast() override;
	void ResetSpecial() override;
	void ResetReload() override;
	void SetHit() override;
	void ResetHit() override;
	void SetZombify() override;
	void ResetZombify() override;

	UFUNCTION(BlueprintCallable)
		void EndAbsorb();

	
};
