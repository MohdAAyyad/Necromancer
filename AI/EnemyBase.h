// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BloodPool.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyAIController.h"
#include "Interactable.h"
#include "Perception/PawnSensingComponent.h"
#include "StatusEffects.h"
#include "TimerManager.h"
#include "../EXPManager.h"
#include "EnemyBase.generated.h"

UCLASS()
class NECROMANCER_API AEnemyBase : public ACharacter, public IInteractable
{
	GENERATED_BODY()

protected:
	// Sets default values for this character's properties
	AEnemyBase();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = Behavior)
	    class UBehaviorTree* enemyBT;

protected:

	class ANecromancerCharacter* player; //Get a reference to the player //Used to allow for zombification

	UPROPERTY(VisibleAnywhere, Category = Perception)
		class UPawnSensingComponent* sense;

	class AEnemyAIController* aiController;

	UPROPERTY(EditAnywhere, Category = Bloodpools)
		TArray<TSubclassOf<class ABloodPool>> bloodPools; //Contains blueprints for tainted and untainted BPs

	UPROPERTY(EditAnywhere, Category = Explosion)
		TSubclassOf<class ABloodExplosion> explosion; //Contains blueprints for tainted and untainted BPs

	UPROPERTY(EditAnywhere, Category = Stats)
		float hp = 100.0f;
	UPROPERTY(EditAnywhere, Category = Stats)
		float maxHP = 100.0f;
	UPROPERTY(EditAnywhere, Category = Stats)
		int exp = 40;
	bool hasAddedEXP = false; //If the player turns the enemy into a zombie, don't get more exp once you die again

	UPROPERTY(EditAnywhere, Category = Stats)
		float baseDamage = 15.0f;
	UPROPERTY(EditAnywhere, Category = Stats)
		float damageModifier = 1.0f; //This increases the higher the player's level is
	UPROPERTY(EditAnywhere, Category = Animation)
		bool bDead = false;

	bool bTypeOfBPToSpawn = false; //False is untainted. True is tainted

	UPROPERTY(EditAnywhere, Category = Bloodpools)
		USceneComponent* bloodPoonSpawnLoc;

	EStatusEffects currentStatusEffect;
	FTimerHandle statusEffectTimerHandle;
	float statusEffectDuration;

	FTimerHandle zombifyTimerHandle;
	float zombifyDuration;
	AEnemyBase* enemyForZombie;//enemy to attack as a zombie. Needed to make sure zombies don't fight each other


	virtual void WhoToLookFor(APawn* pawn_); //Who should the enemy look for? Affeted by zombification
	void AddEXP(); //Calls EXP Manager's UpdateEXP
	UFUNCTION()
		virtual void OnSeePlayer(APawn* pawn_);

	//Patrol
	FVector moveLoc;
	float patrolRadius;
	bool hasPickedApatrolDestination;
	virtual void Patrol();

	APawn* permenantTarget; //Doesn't turn to nullptr when the enemy loses sight of the target. Used when casting spells that do not care about cover


public:
	virtual void TakeRegularDamage(float damage_) {};
	virtual void TakeSpellDamage(float damage_) {};
	virtual void TakeSpellDamage(float damage_, EStatusEffects effect_, float duration_) {};
	virtual EStatusEffects GetCurrentStatusEffect() { return currentStatusEffect; };
	virtual void ApplyStatusEffect();
	virtual void SpawnBloodPool();

	virtual bool React() override { return false; };
	virtual void Interact() override {};
	virtual void Interact(float& value_, bool bp_) override {};
	void WallAction(FRotator rotate_) override {};

	virtual float GetDistanceToPlayer();
	virtual bool IsDead() { return bDead; };
	virtual void Zombify() {};
	virtual void EndZombify() {};
	virtual void Death() {};
	virtual void ActivateZombie() {};

	UPROPERTY(EditAnywhere, Category = Animation)
		bool bZombie = false;

	void DelayedExplosion();
	void Explode(); //Spawns the blood explosion projectile
	void DestroyBody() {Destroy();};

};
