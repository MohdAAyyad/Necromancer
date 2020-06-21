// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyBase.h"
#include "RavenAnimInstance.h"
#include "../Spells/Blood/SummonBase.h"
#include "EnemyProjectile.h"
#include "../BloodPool.h"
#include "Raven.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API ARaven : public AEnemyBase
{
	GENERATED_BODY()
	
protected:
	ARaven();
	void BeginPlay() override;
	void Tick(float deltaTime_) override;

	UPROPERTY(EditAnywhere, Category = Stats)
		float fireRate;

	FTimerHandle fireRateHandle;

	UPROPERTY(EditDefaultsOnly, Category = Spells)
		TSubclassOf<ASummonBase> summon;

	UPROPERTY(EditDefaultsOnly, Category = Spells)
		TSubclassOf<AEnemyProjectile> aimProj;

	UPROPERTY(EditAnywhere, Category = Spells)
		ABloodPool* skeletonSpawnBP; //When the ravens are turned undead, the first thing they do is summon a skeleton and we need a BP reference for that

	UPROPERTY(EditDefaultsOnly, Category = Spells)
		USceneComponent* aimProjSpawnLoc;

	UPROPERTY(EditAnywhere, Category = Spells)
		UParticleSystem* magicCircle; //A magic circle is spawned on the bloodpool before the spell

	URavenAnimInstance* animInstance;

	void EndZombify() override;
	void Attack();
	bool bAttacking;

public:
	void OnSeePlayer(APawn* pawn_) override;
	void TakeRegularDamage(float damage_) override;
	void SpawnAimProj();
	void SpawnSkeleton();
	void PlaySpawnParticles();
	void Zombify() override;
	void ActivateZombie() override;
	void DestroySelf();

};
