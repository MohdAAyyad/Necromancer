// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyBase.h"
#include "Components/BoxComponent.h"
#include "EnemyProjectile.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
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

	virtual void TakeRegularDamage(float damage_) override;
	virtual void TakeSpellDamage(float damage_) override;
	virtual void TakeSpellDamage(float damage_, EStatusEffects effect_, float duration_) override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		USceneComponent* castSpellLocation;

public:
	AKnight();
	EKnightState currentState;
	virtual void OnSeePlayer(APawn* pawn_) override;
	virtual void Tick(float DeltaTime) override;
	void EndSpecial();
	void distactingZombieIsDead() override;


private :

	//Special
	bool hasCastSpecial;
	float specialModifier; //Knight's special increases the damage base modifier
	bool hasUsedCastFireBall; //If used, don't spam, go for melee

	float acceptableMeleeDistance;//Less than this, do melee attack

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

	UPROPERTY(EditAnywhere, Category = "AttackHitBox")
		UBoxComponent* attackHitBox;
	float timeToDisableAttackHitBox;

	class UKnightAnimInstance* animInstance;

	void CheckForSpecial();
	UPROPERTY(EditDefaultsOnly, Category = "Casting")
		TSubclassOf<AEnemyProjectile> castProjectile;

	void KnightAttack();

protected:

	//Strafe
	float acceptableStrafeDistance; //If the player is close to you, do not strafe, just continue attacking
	virtual void Strafe();
	

	UPROPERTY(EditAnywhere, Category = Stats)
	float acceptableAttackDistance; //The distance after which the state switches to attacking
	virtual void ToPlayer();

	float reloadTime;
	FTimerHandle attackTimerHandle;
	UPROPERTY(EditAnywhere, Category = "Melee Impact")
		UParticleSystem* meleeImpact;
	UPROPERTY(EditAnywhere, Category = "Melee Impact")
		USoundBase* meleeImpactSound;
	UPROPERTY(EditAnywhere, Category = "Melee Impact")
		float meleeImpactVolume;
	UPROPERTY(EditAnywhere, Category = "Melee Impact")
		float meleeImpactPitch;

	//Cast
	float acceptableCastDistance; //More than this, calculate cast chance


	virtual void Death() override;
	virtual void Zombify() override;
	virtual void EndZombify() override;

public:
	virtual void Reload();
	virtual void EndReload();

	virtual void EnableHitBox();

	virtual void SpawnCastProjectile();

	virtual void EndHit();

	virtual void ActivateZombie() override;


	
};
