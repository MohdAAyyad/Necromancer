// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/ArrowComponent.h"
#include "HUD/PlayerHUD.h"
#include "GameFramework/PlayerController.h"
#include "AimProjectile.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Animation/LeahAnimInstance.h"
#include "Interactable.h"
#include "DrawDebugHelpers.h"
#include "Spells.h"
#include "SpellsInventory.h"
#include "SpellCheck.h"
#include "StatusEffects.h"
#include "SpellConjuror.h"
#include "HUD/PlayerUIController.h"
#include "PlayerStats.h"
#include "PlayerCameraShake.h"
#include "DialogueHandle.h"
#include "NecromancerCharacter.generated.h"

UCLASS(config=Game)
class ANecromancerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ANecromancerCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	virtual void Tick(float deltaTime_) override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//Custom

public:

	enum class EPlayerState : unsigned short
	{
		IDLE,
		AIM,
		DASH,
		HIT,
		DEATH
	};

	bool bCastingSpell; //Used to make sure the player doesn't move while casting
protected:
	void BeginPlay() override;

	//Camera shake
	UPROPERTY(EditAnywhere, Category = "Camera Shake")
		TSubclassOf<UPlayerCameraShake>cameraShake;

	//Aiming
	UPROPERTY(VisibleAnywhere)
	bool bAim;
	EPlayerState currentState;
	EPlayerState prevState;
	void FlipAimState();

	float lineCastLength;
	FCollisionQueryParams colParams;
	IInteractable* interactable;
	bool bAimingAtABloodPool;
	bool bAimingAtEnemy;//Need to know when aiming at an enemy for corpse manipulator spells
	FVector bloodSpellLocation;
	bool corpseSpell; //Check if we're doing a corpse spell or a regular blood spell
	class AEnemyBase* enemy;


	APlayerHUD* hud;
	APlayerController* playerController;

	//Projectile
	UPROPERTY(EditAnywhere)
		USceneComponent* projectileSummonLocation;
	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* aimPartilces;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AAimProjectile> projectile;


	UFUNCTION()
		void AimInteract();
	UFUNCTION()
		void AimInteractHeal();

	ULeahAnimInstance* animInstance;

	//Stats
	UPROPERTY(EditAnywhere)
	UPlayerStats* stats;

	EAimSpells currentAimSpell; //Updated from spells inventory with the player's input --> Player presses 1 for example --> call use spell and populate the variable --> play animation --> pass in the variables to the conjuror
	EBloodSpells currentBloodSpell;//Updated from spells inventory with the player's input
	EInnateSpells currentInnateSpell;//Updated from spells inventory with the player's input --> This is equipped directly, not through a use function --> Confusing a bit, but it's because there's always only one equipped innate skill
	EStatusEffects currentStatusEffect;//Updated by inventory UseSpell functions
	EStatusDuration currentStatusDuration; //Updated by leveling up. Starts at MIN and scales with the player's stats
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USpellConjuror* conjuror;

	//UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPlayerUIController* uictrl;

	UFUNCTION(BlueprintCallable)
		UPlayerUIController* GetUIController() const;

	//Dashing

	UPROPERTY(EditAnywhere)
		float dashDistance;

	UPROPERTY(EditAnywhere)
		UParticleSystem* magicCircle; //A magic circle is spawned on the bloodpool before the spell

	UFUNCTION()
		void Dash();
	FVector dashVec;
	float dashTime;
	FTimerHandle dashTimeHandler;

	//Spells
	bool increaseHP;
	bool usingASpellSummon;
	float spellBaseDamage;
	EStatusEffects effect;
	//Spell functions
	void CallAimSpell(int index_);
	void CallBloodSpell(int index_);
	void CallInnateSpell();

	void CallAimSpell0();
	void CallAimSpell1();
	void CallAimSpell2();
	void CallAimSpell3();

	void CallBloodSpell0();
	void CallBloodSpell1();
	void CallBloodSpell2();
	void CallBloodSpell3();



	//------Animation functions

	UFUNCTION(BlueprintCallable)
	void AnimShoot();
	UFUNCTION(BlueprintCallable)
	void AnimShootProjectile();

	UFUNCTION(BlueprintCallable)
		void ConjurAimSpell();
	UFUNCTION(BlueprintCallable)
		void ConjurBloodSpell();

	public:
		void TakeDamage(float damage_);

		void MoveDueToDash(); //Called from animation
		void EndDash();

		void EndHit();

		void ConjurSpell();

		//Wall
		UFUNCTION()
			void SpawnWall();
		void PlayWallAnimation();

		//Dialogue
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
			UDialogueHandle* dialogueHandle;


		UPROPERTY(EditAnywhere, Category = "DialogueBoxCollision")
			UBoxComponent* dialogueCollisionBox;

		public:
			void InitiateAutoDialogue();

};
