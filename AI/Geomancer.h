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
	void OnSeePlayer(APawn* pawn_) override;
	void Tick(float DeltaTime) override;
	void TakeRegularDamage(float damage_) override;
	void TakeSpellDamage(float damage_) override;
	void TakeSpellDamage(float damage_, EStatusEffects effect_, float duration_) override;

	void EndAttack();
	//Sitting
	void EndSitting();

	void SpawnCastProjectile() override;
	void SpawnCastProjectile1();
	void SpawnSpecialProjectile();

	void EvadeTeleport();


	void ActivateZombie() override;

protected:

	void BeginPlay() override;
	void Death() override;
	void Zombify() override;
	void EndZombify() override;
	void EndReload() override;

	float GetDistanceToPlayer() override;

		

	UPROPERTY(EditAnywhere, Category = "Current State") //The current state is editable in the editor as the enemy could start sitting down or patrolling
		EGeomancerState currentState;
	class UGeoAnimInstance* animInstance;

	//Attack
	void Attack();
	bool bHasChosenSpell;
	int prevChosenSpell; //Keep track of which spell was chosen last to prevent spam

	UPROPERTY(EditDefaultsOnly, Category = "Casting")
		TArray<TSubclassOf<AEnemyProjectile>> projectiles;

	UPROPERTY(EditAnywhere, Category = "Casting")
		USceneComponent* castSpell1Location;

	void CalculateEvade(); //Run a random chance and see if you need to evade. Called inside take damage functions
	UPROPERTY(EditAnywhere, Category = "Evasion")
		float evadeRange; //How far forward can you go?
	UPROPERTY(EditAnywhere, Category = "Evasion")
		UParticleSystemComponent* evadeParticles;

	bool bhasChosenEvadeDestination; //Makes sure the line cast stops once we've found a destination
	FVector evadeDestination;
};
