// Fill out your copyright notice in the Description page of Project Settings.


//TODO
/*
* Create an animation instance
* Create a skeleton class
* Create a behvavior tree for summon base :Move-Attack-Reload-Die
* Fill the function related to the animations
* Summon the skeleton
*/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI/EnemyAIController.h"
#include "Perception/PawnSensingComponent.h"
#include "StatusEffects.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "AimProjectile.h"
#include "AI/EnemyBase.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Animation/Summon/SummonAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "SummonBase.generated.h"

UCLASS()
class NECROMANCER_API ASummonBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASummonBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = Behavior)
		class UBehaviorTree* summonBT;

	void SetSummonLevel(int level_);
	void ActivateSummon();

	virtual void EnableAttackHitBox(); //Called from animation
	virtual void DisableAttackHitBox(); //Called from animation
	virtual void LeaveScene(); //Called from animation after Death

protected:

	USummonAnimInstance* animInstance;

	bool bActive = false; //When true, summon functions. Turned to true by animation instance

	AEnemyBase* target;

	FTimerHandle dieTimer;
	float timeToDie = 40.0f;;

	FTimerHandle attackTimer;
	float timeToDisableAttackHitBox = 0.3f;
	float reloadTime = 0.2f;
	bool bIsReloading = false;

	UPROPERTY(EditAnywhere, Category = Perception)
		 UPawnSensingComponent* sense;

	 AEnemyAIController* aiController;
	 
	 UPROPERTY(EditAnywhere, Category = AttackHitBox)
		 UBoxComponent* attackHitBox;

	 UPROPERTY(EditAnywhere, Category = AttackHitBox)
		 TArray<TSubclassOf<AAimProjectile>> deathObjects; //As the player's level increases, summons can spawn a projectile upon their death

	 UPROPERTY(EditAnywhere, Category = Stats)
		 int level = 1;

	UPROPERTY(EditAnywhere, Category = Stats)
		float hp = 100.0f;
	UPROPERTY(EditAnywhere, Category = Stats)
		float maxHP = 100.0f;

	UPROPERTY(EditAnywhere, Category = Stats)
		float baseDamage = 25.0f;
	UPROPERTY(EditAnywhere, Category = Stats)
		float damageModifier = 1.0f; //This improves when increasing the magic base damage stat of the player
	UPROPERTY(EditAnywhere, Category = Stats)
		float attackDistance = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Impact")
		UParticleSystem* impact;

	UFUNCTION()
		virtual void OnSeeEnemy(APawn* pawn_);
	virtual void TakeDamage(float damage_);
	virtual void Attack();
	virtual void EndReload();
	virtual void Die();
	virtual void DestroySummon();
	virtual float GetDistanceToEnemy();

	UFUNCTION()
		virtual void OnAttackBoxOverlap(UPrimitiveComponent* overlappedComponent_,
			AActor* otherActor_,
			UPrimitiveComponent* otherComp_,
			int32 otherBodyIndex_,
			bool bFromSweep_,
			const FHitResult &sweepResult_);

};
