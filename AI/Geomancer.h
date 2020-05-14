// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Knight.h"
#include "Geomancer.generated.h"

/**
 * 
 */

UENUM()
enum class EGeomancerState : uint8
{
	PATROLLING,
	SITTING,
	TOPLAYER,
	ATTACKING,
	CASTING,
	RELOADING,
	EVADING,
	CANISEEPLAYER, //Makes sure enemy doesn't check the bHasSeenPlayer boolean before onSeeRuns
	DEATH,
};

UCLASS()
class NECROMANCER_API AGeomancer : public AKnight
{
	GENERATED_BODY()
	

public:
	AGeomancer();
	virtual void OnSeePlayer(APawn* pawn_) override;
	virtual void Tick(float DeltaTime) override;
	virtual void TakeRegularDamage(float damage_) override;
	virtual void TakeSpellDamage(float damage_) override;
	virtual void TakeSpellDamage(float damage_, EStatusEffects effect_, float duration_) override;

	void EndAttack();
	//Sitting
	void EndSitting();

	virtual void SpawnCastProjectile() override;
	virtual void SpawnCastProjectile1();
	virtual void SpawnSpecialProjectile();

	void EvadeTeleport();


	virtual void ActivateZombie() override;

protected:

	virtual void BeginPlay() override;
	virtual void Death() override;
	virtual void Zombify() override;
	virtual void EndZombify() override;
	virtual void EndReload() override;

	virtual float GetDistanceToPlayer() override;

		
private:
	UPROPERTY(EditAnywhere, Category = "Current State") //The current state is editable in the editor as the enemy could start sitting down or patrolling
		EGeomancerState currentState;
	class UGeoAnimInstance* animInstance;

protected:
	//Attack
	virtual void Attack();
	bool bHasChosenSpell;
	int prevChosenSpell; //Keep track of which spell was chosen last to prevent spam

	UPROPERTY(EditDefaultsOnly, Category = "Casting")
		TArray<TSubclassOf<AEnemyProjectile>> projectiles;

	UPROPERTY(EditAnywhere, Category = "Casting")
		USceneComponent* castSpell1Location;

private:
	void CalculateEvade(); //Run a random chance and see if you need to evade. Called inside take damage functions
	UPROPERTY(EditAnywhere, Category = "Evasion")
		float evadeRange; //How far forward can you go?
	UPROPERTY(EditAnywhere, Category = "Evasion")
		UParticleSystemComponent* evadeParticles;

	bool bhasChosenEvadeDestination; //Makes sure the line cast stops once we've found a destination
	FVector evadeDestination;
};
