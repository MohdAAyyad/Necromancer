// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIAnimeInstanceBase.h"
#include "GeoAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API UGeoAnimInstance : public UAIAnimeInstanceBase
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
		void UpdateAnimationProperties();

	UGeoAnimInstance();

	class AGeomancer* geomancer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float forwardSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bCast0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bCast1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bSpecial0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bReload;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bDeath;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bHit;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bSitting;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bEvading;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(int index_);

	UFUNCTION(BlueprintCallable)
		void EvadeTeleport();

public:
	void Reload() override;//Every enemy has a reload animation.
	void Death() override; //Every enemy needs to die
	void SpawnBP() override;
	void NextCast() override;
	void NextCast1(); //Second spell
	void NextSpecial() override; //Special - I don't care about cover - spell
	//Reset functions reset the active boolean of each type
	void ResetCast() override;
	void ResetReload() override;
	void SetHit() override;
	void ResetHit() override;
	void SetZombify() override;
	void ResetZombify() override;

	void SetSitting();
	void ResetSitting();

	void SetEvading();
	void ResetEvading();


};
