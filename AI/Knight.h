// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyBase.h"
#include "Components/BoxComponent.h"
#include "EnemyProjectile.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Knight.generated.h"

/**
 * The knight is a ground based enemy. Once he sees the player, he strafes, then decides whether to 
 * go to the player and perform a melee attack or cast a fireball spell.
 * Once the knight reaches 40% or less of his health, he uses his special which restores a bit of health, increases damage
 * and makes the knight use melee attacks exclusively
 */
UCLASS()
class NECROMANCER_API AKnight : public AEnemyBase
{
	GENERATED_BODY()

public:
	enum class EKnightState : unsigned short
	{
		PATROLLING,
		STRAFING,
		TOPLAYER,
		ATTACKING,
		CASTING,
		RELOADING,
		SPECIAL,
		DEATH,
	};

	void TakeRegularDamage(float damage_) override;
	void TakeSpellDamage(float damage_, EStatusEffects effect_, float duration_) override;

protected:

	void BeginPlay() override;
	class UKnightAnimInstance* animInstance;
	UPROPERTY(EditAnywhere)
	USceneComponent* castSpellLocation;



public:
	AKnight();
	EKnightState currentState;
	void SpawnBloodPool() override;
	void OnSeePlayer(APawn* pawn_) override;
	void Tick(float DeltaTime) override;

protected:


	//Strafe
	FVector moveLoc;
	float acceptableStrafeDistance; //If the player is close to you, do not strafe, just continue attacking
	void Strafe();

	//Patrol
	float patrolRadius;
	bool hasPickedApatrolDestination;
	void Patrol();

	//Cast
	float acceptableCastDistance; //More than this, calculate cast chance
	float acceptableMeleeDistance;//Less than this, do melee attack
	bool hasUsedCastFireBall; //If used, don't spam, go for melee

	//Special
	bool hasCastSpecial;
	float specialModifier; //Knight's special increases the damage base modifier

	UPROPERTY(EditAnywhere, Category = Stats)
	float acceptableAttackDistance; //The distance after which the state switches to attacking
	void ToPlayer();

	float reloadTime;
	FTimerHandle attackTimerHandle;

	UPROPERTY(EditAnywhere, Category = "AttackHitBox")
	UBoxComponent* attackHitBox;
	float timeToDisableAttackHitBox;

	UPROPERTY(EditDefaultsOnly, Category = "Casting")
		TSubclassOf<AEnemyProjectile> castProjectile;

	UPROPERTY(EditAnywhere, Category = "Special")
		UParticleSystemComponent* specialParticles;
	
	UFUNCTION()
		void AttackOverlap
		(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult &SweepResult);


	void Death() override;
	void Zombify() override;
	void EndZombify() override;
	void WhoToLookFor(APawn* pawn_) override;

	void CheckForSpecial();


public:
	void Reload();
	void EndReload();

	void EnableHitBox();

	void SpawnCastProjectile();

	void EndHit();
	void EndSpecial();

	void ActivateZombie() override;


	
};
