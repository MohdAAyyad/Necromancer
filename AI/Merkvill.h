// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyBase.h"
#include "EnemyProjectile.h"
#include "MerkvillAnimInstance.h"
#include "MerkvillShield.h"
#include "../NPC/MerkvillEndDialogue.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Merkvill.generated.h"


UENUM()
enum class EMerkvillState : uint8
{
	TALKING, //Dummy state, meant for when the player is still talking to the boss before the fight starts
	IDLE,
	ATTACKING,
	RELOADING,
	DEATH,
	EVADING
};

UENUM()
enum class EMerkvillPhase : uint8
{
	PHASE0, //Cannot be damaged. Player needs to use dead workers on each side of the map to break his armor. Shoots fireballs
	PHASE1, //Shoots fireballs and activates traps
	PHASE2, //More of the same, starts with an explosion that kills all the undead. Summon Fire tornadoes. Near the end of its life, starts the explosion again but takes quite during which the player needs to kill him
};
UCLASS()
class NECROMANCER_API AMerkvill : public AEnemyBase
{
	GENERATED_BODY()

public:

	void TakeRegularDamage(float damage_) override;
	void TakeSpellDamage(float damage_) override;
	void TakeSpellDamage(float damage_, EStatusEffects effect_, float duration_) override;
	void TakeSpellDamageFromZombie(APawn* zombie_, float damage_) override;
	void StartFight(); //Called from the dialogue box before it dissapears.
	void SetShieldRef(AMerkvillShield* ref_);

protected:
	AMerkvill();

	UPROPERTY(EditAnywhere, Category = Stats, BlueprintReadOnly)
		float armor;
	UPROPERTY(EditAnywhere, Category = Stats)
		float maxArmor;

	UFUNCTION(BlueprintCallable)
		float GetArmorPercent() { return armor / maxArmor; };

	UPROPERTY(EditAnywhere, Category = Stats, BlueprintReadOnly)
		bool bFightHasStarted;

		AMerkvillShield* shieldRef;

	UPROPERTY(EditAnywhere, Category = "Attacks")
		TArray<TSubclassOf<AEnemyProjectile>> spells;

	void OnSeePlayer(APawn* pawn_) override;

	UMerkvillAnimInstance* animInstance;
	EMerkvillState currentState;
	EMerkvillPhase currentPhase;
	

	void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void Death() override;

protected:
	//Evading
	UPROPERTY(EditAnywhere, Category = Evading)
		UParticleSystem* evadeParticles; //Emitted at current and evade locations
	FVector startingLocation; //Used as a reference for teleporting

	void Evade();
	int currentLocationIndex; //0 Starting, 1 forward, 2 backwards


	//Attacking
	FTimerHandle reloadHandle;
	float reloadTime;
	void EndReload();
	void Attack(); //Attacks are based on the current phase

	int attackCount; //Since we have a series of attacks, only when attackCount is zero should we go into reloading

//Fireballs
	FTimerHandle fireRateHandle;
	float fireRate;
	int numberOfFireBalls;
	int maxNumberOfFireBalls;

//Meteors
	int maxNumberOfMeteors;

//Tornado
	int maxNumberOfTornados;

//Explosion
	UPROPERTY(EditAnywhere, Category = ExplosionAttack)
		USphereComponent* explosionSphere;
	UPROPERTY(EditAnywhere, Category = ExplosionAttack)
		UParticleSystemComponent* explosionIndicatorParticles;
	UPROPERTY(EditAnywhere, Category = ExplosionAttack)
		UParticleSystem* explosionImpact;
	UPROPERTY(EditAnywhere, Category = ExplosionAttack)
		float explosionDamage;
	UPROPERTY(EditAnywhere, Category = ExplosionAttack)
		float timeToExplode;

	UPROPERTY(EditAnywhere, Category = ExplosionAttack)
		float timeUntilNextExplosion;
	bool bCanExplode;
	FTimerHandle timeToExplodeHandle;

	UPROPERTY(EditAnywhere, Category = "AudioComponent")
		UAudioComponent* audioComponent;
	UPROPERTY(EditAnywhere, Category = "AudioComponent")
		USoundBase* explosionCharge;
	UPROPERTY(EditAnywhere, Category = "AudioComponent")
		USoundBase* evadeSound;

	UFUNCTION()
		void ExplosionOverlap
		(UPrimitiveComponent* overlappedComponent_,
			AActor* otherActor_,
			UPrimitiveComponent* otherComp_,
			int32 otherBodyIndex_,
			bool bFromSweep_,
			const FHitResult &sweepResult_);

	void CanExplodeAgain();

	//End dialogue
	UPROPERTY(EditAnywhere, Category = "End Dialogue")
		TSubclassOf<AAutoDialogue> endDialogueIndicator;
	UPROPERTY(EditAnywhere, Category = "End Dialogue")
		TSubclassOf<AMerkvillEndDialogue> endDialogueFinal;

	//Quest
	UPROPERTY(EditAnywhere, Category = "Quest")
		FString questName;
	UPROPERTY(EditAnywhere, Category = "Quest")
		FString questDescriptionStartFight;
	UPROPERTY(EditAnywhere, Category = "Quest")
		FString questDescriptionEnd;
public:
	void Reload();
	
	void FireBall();
	void Meteors();
	void Tornado();
	void InitiateExplosion(); //Tells the animation instance to start exploding
	void Explode(); //The actual explosion
	void ResetExplosionSphere();
	
};
