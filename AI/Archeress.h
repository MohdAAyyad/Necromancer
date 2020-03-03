// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyBase.h"
#include "EnemyProjectile.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Archeress.generated.h"

/**
 * The archeress does not move. She's positioned at higher grounds where the player cannot reach.
 * Once she sees the player, she checks if they're close enough, aims, and shoots.
 * She can also cast an arrow spell
 */
UCLASS()
class NECROMANCER_API AArcheress : public AEnemyBase
{
	GENERATED_BODY()
	
public:
	enum class EArcheressState : unsigned short
	{
		IDLE,
		AIM,
		CAST,
		DEATH,
		ZOMBIE
	};

protected:

	void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	class UArcheressAnimInstance* animInstance;

public:
	EArcheressState currentState;
	AArcheress();
	int castChance;
	void SpawnBloodPool() override;
	void OnSeePlayer(APawn* pawn_) override;
	void TakeRegularDamage(float damage_) override;
	void TakeSpellDamage(float damage_, EStatusEffects effect_, float duration_) override;
	void SpawnRegularArrow();
	void DeactivateCastParticles();

protected:
	class ANecromancerCharacter* player;

	UPROPERTY(EditAnywhere, Category = "Distance")
	float attackAcceptableDistance; //Distance before enemy starts attacking

	//Arrow firing timer
	float drawTime; //Time the archeress has the bow drawn before firing the arrows
	FTimerHandle looseTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Arrows")
		TArray<TSubclassOf<AEnemyProjectile>> arrows; //0 regular 1+ spells

	void ShootArrow(); //Called after drawing arrows

	UPROPERTY(EditAnywhere, Category = "Cast")
		UParticleSystemComponent* castParticles;

	UPROPERTY(EditAnywhere)
		USceneComponent* arrowSummonLocation;

	void Death() override;
	void Zombify() override;
	void EndZombify() override;
	void WhoToLookFor(APawn* pawn_) override;

public:
	void ActivateZombie() override;



};
