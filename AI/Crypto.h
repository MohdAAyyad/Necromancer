// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Geomancer.h"
#include "Components/AudioComponent.h"
#include "Crypto.generated.h"


#define PORTAL_FIRE_RATE 0.2f;
#define SHOOT_FIRE_RATE  1.0f;
#define ABSORPTION_DURATION 40.0f;

UENUM()
enum class ECryptoState : uint8
{
	IDLE,
	ATTACKING,
	ABSORBING,
	RELOADING,
	DEATH,
	ZOMBIE
};

UCLASS()
class NECROMANCER_API ACrypto : public AGeomancer
{
	GENERATED_BODY()
		
public:
	void ActivateZombie() override;

	void TakeRegularDamage(float damage_) override;
	void TakeSpellDamage(float damage_) override;
	void TakeSpellDamage(float damage_, EStatusEffects effect_, float duration_) override;

	void Absorb(float damage_);
	void EndAbsorb(); //Called from animation instance

	bool IsEnemyAbsorbingAttack() override;

	void SpawnBloodPool() override;

protected:
	ACrypto();
	ECryptoState currentState;
	class UCryptoAnimInstance* animInstance;

	UPROPERTY(EditAnywhere, Category = "Portals")
		TSubclassOf<AEnemyProjectile> portalProjectile; //The projectiles that spawn from the portals

	UPROPERTY(EditAnywhere, Category = "Absorb")
		TSubclassOf<AEnemyProjectile> absorbRay; //The ray the enemy shoots after absorbtion is done

	UPROPERTY(EditAnywhere, Category = "Absorb")
		UParticleSystemComponent* absorbIndicator;


	UPROPERTY(EditAnywhere, Category = "Absorb")
		float absorbSpeed; //How fast do projectiles get absorbed

	UPROPERTY(EditAnywhere, Category = "AudioComponent")
		UAudioComponent* audioComponent;

	int absorbChance; //Calculates the chance to activate attack absorbtion
	float totalDamageAbsorbed;

	 void AbsorbMode();
	 bool CalculateAbsorbChance();
	 void Attack() override;
	 void BeginPlay() override;
	 void Tick(float DeltaTime) override;
	 void Death() override;
	 void Zombify() override;
	 void EndZombify() override;
	 void EndReload() override;	

	 bool bHasSummonedPortals;
	 float fireRate;
	 int numberOfAttacksSoFar; 
	 int maxNumberOfAttacksBeforeReload;
	 int numberOfPortalsToSummon; //Increases when Crypto loses health
	 int numberOfPortalsSofar;
	 UPROPERTY(EditAnywhere, Category = "Absorb")
	 float absorptionDuration;

	 TArray<AEnemyProjectile*> spawnedPortals;
	 TArray<AEnemyProjectile*> actualProjectiles;


		 TArray<USceneComponent*> portalLocations;

		 UPROPERTY(EditAnywhere, Category = "PortalLocs")
			 USceneComponent* portalLoc0;
		 UPROPERTY(EditAnywhere, Category = "PortalLocs")
			 USceneComponent* portalLoc1;
		 UPROPERTY(EditAnywhere, Category = "PortalLocs")
			 USceneComponent* portalLoc2;
		 UPROPERTY(EditAnywhere, Category = "PortalLocs")
			 USceneComponent* portalLoc3;
		 UPROPERTY(EditAnywhere, Category = "PortalLocs")
			 USceneComponent* portalLoc4;
		 UPROPERTY(EditAnywhere, Category = "PortalLocs")
			 USceneComponent* portalLoc5;
		 UPROPERTY(EditAnywhere, Category = "PortalLocs")
			 USceneComponent* portalLoc6;
		 UPROPERTY(EditAnywhere, Category = "PortalLocs")
			 USceneComponent* portalLoc7;
		 UPROPERTY(EditAnywhere, Category = "PortalLocs")
			 USceneComponent* portalLoc8;
		 UPROPERTY(EditAnywhere, Category = "PortalLocs")
			 USceneComponent* portalLoc9;
		 UPROPERTY(EditAnywhere, Category = "PortalLocs")
			 USceneComponent* beamLoc;


		 UPROPERTY(EditAnywhere, Category = "Abosorption")
			 USphereComponent* outerSphere;
		 UPROPERTY(EditAnywhere, Category = "Abosorption")
			 USphereComponent* innerSphere;

		 UFUNCTION()
			 void OuterSphereOnOverlap(UPrimitiveComponent* overlappedComponent_,
				 AActor* otherActor_,
				 UPrimitiveComponent* otherComp_,
				 int32 otherBodyIndex_,
				 bool bFromSweep_,
				 const FHitResult &sweepResult_);

		 UFUNCTION()
			 void InnerSphereOnOverlap(UPrimitiveComponent* overlappedComponent_,
				 AActor* otherActor_,
				 UPrimitiveComponent* otherComp_,
				 int32 otherBodyIndex_,
				 bool bFromSweep_,
				 const FHitResult &sweepResult_);
		 
		 
		 void ActivateSpheres();
		 void DeactivateSpheres();

		 FTimerHandle absorbHandle;
		// bool bHasAlreadyBeenZombified; //We need this bool to destroy the enemy after the zombification has ended. 
								   //We need to destroy the enemy AFTER all the portals have been destroyed, and we cannot
								  //just destroy it after spawnBP has ended as we will not know whether the portals 
								  //were destroyed or not. bDead and bZombie are not enough in this case either as 
								  //bDead will always be true once the enemy dies once and the series of checks inside the 
								  //Attack() function will render this condition to happen (if dead and zombie --> attack) 
								  //so I have to somehow let the code know that I'm indeed dead, but I used to be a zombie 
								  //and now I need to be destroyed

		 void ClearAndDestroy(); //Clears portals and destroys enemy

};
