// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "NecromancerCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "TimerManager.h"
#include "AI/EnemyBase.h"
#include "BloodWall.h"
#include "NecromancerGameMode.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ANecromancerCharacter

ANecromancerCharacter::ANecromancerCharacter()
{

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//Custom
	bAim = false;
	prevState = currentState = EPlayerState::IDLE;
	hud = nullptr;
	playerController = nullptr;
	animInstance = nullptr;
	colParams = FCollisionQueryParams::DefaultQueryParam;
	interactable = nullptr;
	bAimingAtABloodPool = false;
	bAimingAtEnemy = false;

	projectileSummonLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Summon Location"));
	projectileSummonLocation->SetupAttachment(RootComponent,FName(TEXT("RightHandSocket")));

	aimPartilces = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Aim Particle System"));
	aimPartilces->SetupAttachment(projectileSummonLocation);

	conjuror = CreateDefaultSubobject<USpellConjuror>(TEXT("Conjuror"));

	stats = CreateDefaultSubobject<UPlayerStats>(TEXT("Player Stats"));

	uictrl = CreateDefaultSubobject<UPlayerUIController>(TEXT("UI Controller"));

	dialogueHandle = CreateDefaultSubobject<UDialogueHandle>(TEXT("Dialogue Handle"));

	dialogueCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DialogueBoxCollision"));
	dialogueCollisionBox->SetGenerateOverlapEvents(true);
	dialogueCollisionBox->SetCollisionProfileName("Dialogue");
	dialogueCollisionBox->SetupAttachment(RootComponent);


	//Stats
	spellBaseDamage = 0.0f;
    lineCastLength = 1500.0f;
	dashDistance = 1000.0f;
	dashTime = 0.7495f;

	currentAimSpell = EAimSpells::AIMNONE;
	currentBloodSpell = EBloodSpells::BLOODNONE;
	currentInnateSpell = EInnateSpells::INNATENONE;
	currentStatusEffect = EStatusEffects::NONE; //Out of these 5, this is the only one that marks the status effect of the player
	currentStatusDuration = EStatusDuration::MIN;

	increaseHP = false;
	usingASpellSummon = false;
	effect = EStatusEffects::NONE;
	bCastingSpell = false;

	bloodSpellLocation = FVector::ZeroVector;
	enemy = nullptr;
	GetCharacterMovement()->MaxWalkSpeed = 900.0f;
}

void ANecromancerCharacter::BeginPlay()
{
	Super::BeginPlay();
	playerController = GetWorld()->GetFirstPlayerController();
	hud = Cast<APlayerHUD>(playerController->GetHUD());

//	if (hud)
		//hud->SetUICTRL(uictrl);
	if (aimPartilces)
		aimPartilces->DeactivateSystem();

	animInstance = Cast<ULeahAnimInstance>(GetMesh()->GetAnimInstance());
	SpellsInventory::GetInstance(); //Initiate instance
	SpellCheck::GetInstance();

	if (uictrl)
	{
		if (stats)
			uictrl->SetPlayerStatsPtr(stats);
	}

	ANecromancerGameMode* gameMode = Cast<ANecromancerGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode)
	{
		if (gameMode->GetStartingPosition() != FVector::ZeroVector)
		{
			SetActorLocation(gameMode->GetStartingPosition());
		}
	}	

	if (dialogueHandle)
	{
		dialogueHandle->SetUICTRL(uictrl);
	}

	if (dialogueCollisionBox)
	{
		dialogueCollisionBox->OnComponentBeginOverlap.AddDynamic(dialogueHandle, &UDialogueHandle::DialogueOverlap);
		dialogueCollisionBox->OnComponentEndOverlap.AddDynamic(dialogueHandle, &UDialogueHandle::EndDialogueOverlap);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ANecromancerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ANecromancerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANecromancerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &ANecromancerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ANecromancerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &ANecromancerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ANecromancerCharacter::LookUpAtRate);

	//Custom

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this,&ANecromancerCharacter::FlipAimState);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ANecromancerCharacter::FlipAimState);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ANecromancerCharacter::AimInteract);
	PlayerInputComponent->BindAction("Heal", IE_Pressed, this, &ANecromancerCharacter::AimInteractHeal);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ANecromancerCharacter::AnimShoot);
	PlayerInputComponent->BindAction("Talk", IE_Pressed, dialogueHandle, &UDialogueHandle::ProcessInput);

	//Spells
	PlayerInputComponent->BindAction("Aim1", IE_Pressed, this, &ANecromancerCharacter::CallAimSpell0);
	PlayerInputComponent->BindAction("Aim2", IE_Pressed, this, &ANecromancerCharacter::CallAimSpell1);
	PlayerInputComponent->BindAction("Aim3", IE_Pressed, this, &ANecromancerCharacter::CallAimSpell2);
	PlayerInputComponent->BindAction("Aim4", IE_Pressed, this, &ANecromancerCharacter::CallAimSpell3);

	PlayerInputComponent->BindAction("Blood1", IE_Pressed, this, &ANecromancerCharacter::CallBloodSpell0);
	PlayerInputComponent->BindAction("Blood2", IE_Pressed, this, &ANecromancerCharacter::CallBloodSpell1);
	PlayerInputComponent->BindAction("Blood3", IE_Pressed, this, &ANecromancerCharacter::CallBloodSpell2);
	PlayerInputComponent->BindAction("Blood4", IE_Pressed, this, &ANecromancerCharacter::CallBloodSpell3);

	//UI
	PlayerInputComponent->BindAction("Pause", IE_Pressed, uictrl, &UPlayerUIController::PauseGame);
	
	//Dash
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ANecromancerCharacter::Dash);

	//Wall
	PlayerInputComponent->BindAction("Wall", IE_Pressed, this, &ANecromancerCharacter::PlayWallAnimation);
}


void ANecromancerCharacter::Tick(float deltaTime_)
{
	Super::Tick(deltaTime_);

	if (currentState == EPlayerState::AIM)
	{
		FHitResult aimHit;
		FVector start = FollowCamera->GetComponentLocation();
		FVector forward = FollowCamera->GetForwardVector();
	
		if (GetWorld()->LineTraceSingleByChannel(aimHit, start, forward*lineCastLength + start, ECC_WorldDynamic, colParams))
		{
			//DrawDebugLine(GetWorld(), start, forward*lineCastLength + start, FColor(0, 255, 0), false);
			interactable = Cast <IInteractable>(aimHit.GetActor());
;
			if (interactable != nullptr)
			{
				if (interactable->React()) //True == bloodPool, false == enemy
				{
					hud->aimingAtBP();
					if (!bAimingAtABloodPool)
						bAimingAtABloodPool = true;
					if(bAimingAtEnemy)
						bAimingAtEnemy = false;

					enemy = nullptr;
				}
				else
				{
					hud->aimingAtEnemy();
					if (bAimingAtABloodPool)
						bAimingAtABloodPool = false;

					if(!enemy) //Could end up being a problem if both enemies are really close to each other such the the pointer doesn't get a chance to be nullified
						enemy = Cast<AEnemyBase>(aimHit.GetActor());

					if(enemy) //Only flip the boolean to true if the enemy is actually dead
						if(enemy->IsDead())
							if (!bAimingAtEnemy)
								bAimingAtEnemy = true;

				}
				bloodSpellLocation = aimHit.GetActor()->GetActorLocation();
			}
			else
			{
				//If aiming at an actor that doesn't cast correctly, reset
				hud->aimingAtIdle();
				bAimingAtABloodPool = false;
				bAimingAtEnemy = false;
				enemy = nullptr;
			}
		}
		else
		{ 
			//If aiming at nothing, reset
			hud->aimingAtIdle();
			bAimingAtABloodPool = false;
			bAimingAtEnemy = false;
			interactable = nullptr;
			enemy = nullptr;
		}
	}
}

#pragma region Movement

void ANecromancerCharacter::TurnAtRate(float Rate)
{

	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ANecromancerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ANecromancerCharacter::MoveForward(float Value)
{
	if (!bCastingSpell && !dialogueHandle->IsInDialogue())
	{
		if ((Controller != NULL) && (Value != 0.0f) && currentState != EPlayerState::HIT)
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
	}
}

void ANecromancerCharacter::MoveRight(float Value)
{
	if (!bCastingSpell && !dialogueHandle->IsInDialogue())
	{
		if ((Controller != NULL) && (Value != 0.0f) && currentState != EPlayerState::HIT)
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
	}
}


#pragma endregion


//Custom

#pragma region Aim

void ANecromancerCharacter::FlipAimState()
{
	if (bAim) //Exit aim mode
	{
		if (hud)
		{
			hud->bDrawing = false;
			hud->aimingAtIdle();
		}
		bAim = false;
		FollowCamera->FieldOfView = FMath::Lerp(FollowCamera->FieldOfView, 90.0f, 1.0f);
		currentState = EPlayerState::IDLE;
		animInstance->banimAimShoot = false;
		bUseControllerRotationYaw = false;
		aimPartilces->DeactivateSystem();
		if (animInstance)
			animInstance->baimMode = false;
		GetCharacterMovement()->MaxWalkSpeed = 900.0f;
		interactable = nullptr;  //Reset interactable
		bAimingAtABloodPool = false; //Reset bAimingAtABloodPool
		bAimingAtEnemy = false;
		bCastingSpell = false;
	}
	else //Enter aim mode
	{
		if (hud)
			hud->bDrawing = true;
		bAim = true;
		GetController()->SetControlRotation(FollowCamera->GetComponentRotation());
		FollowCamera->FieldOfView = FMath::Lerp(FollowCamera->FieldOfView, 60.0f, 1.0f);
		currentState = EPlayerState::AIM;
		bUseControllerRotationYaw = true;
		aimPartilces->ActivateSystem(true);
		if (animInstance)
		{
			animInstance->baimMode = true;
			animInstance->ResetSpellAnimation(); //Reset all the spell booleans
		}
		GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	}
}

#pragma endregion

#pragma region Interact

void ANecromancerCharacter::AimInteract() //Pressing interact
{
	if (currentState == EPlayerState::AIM)
	{
		if (interactable && bAimingAtABloodPool && !bAimingAtEnemy)
		{
			interactable->Interact(stats->GetBP(), true); //absorb blood points from the blood pool
		}
	}
}


void ANecromancerCharacter::AimInteractHeal() //Pressing interact heal
{
	if (currentState == EPlayerState::AIM)
	{
		if (interactable && bAimingAtABloodPool && !bAimingAtEnemy)
		{
			interactable->Interact(stats->GetHP(), false); //absorb health points from the blood pool
		}
	}
}

#pragma endregion

void ANecromancerCharacter::ConjurSpell()
{
	//Called from animation instance
	//Check which spell to conjur
	if (currentAimSpell != EAimSpells::AIMNONE)
	{
		ConjurAimSpell();
	}
	else if (currentBloodSpell != EBloodSpells::BLOODNONE)
	{
		ConjurBloodSpell();
	}
}

#pragma region CallAndConjurAimSpell
void ANecromancerCharacter::CallAimSpell(int index_)
{
	spellBaseDamage = stats->GetBaseMagicDamage(); //Reset spellBaseDamage
	if (currentState == EPlayerState::AIM)
	{
		if (SpellsInventory::GetInstance() && SpellCheck::GetInstance() && conjuror)
		{
			currentAimSpell = SpellsInventory::GetInstance()->GetEquippedAimSpellForTexture(index_); //Get The spell

			if (SpellCheck::GetInstance()->CheckForAimSpell(currentAimSpell, stats->GetBP(), increaseHP, spellBaseDamage, effect))
			{
				bCastingSpell = true;

					switch (currentAimSpell)
					{
					case EAimSpells::BLOODSHOT:
						animInstance->buseSpell1 = true;
						break;
					case EAimSpells::BLOODROCKET:
						animInstance->buseSpell1 = true;
						break;
					case EAimSpells::BLOODTIMEBOMB:
						animInstance->buseSpell2 = true;
						break;
					default:
						break;

					}
			}
		}
	}
}

void ANecromancerCharacter::ConjurAimSpell() //Called from animation notification
{
	if (currentAimSpell != EAimSpells::AIMNONE)
	{
		conjuror->ConjurAimSpell(currentAimSpell, projectileSummonLocation->GetComponentLocation(), GetViewRotation(), increaseHP, stats->GetHP(), spellBaseDamage, effect, currentStatusDuration, playerController, cameraShake); //Conjur the spell
		currentAimSpell = EAimSpells::AIMNONE; //Reset
	}
}
#pragma endregion

#pragma region CallAndConjurBloodSpell

void ANecromancerCharacter::CallBloodSpell(int index_)
{
	spellBaseDamage = stats->GetBaseMagicDamage();
	if (currentState == EPlayerState::AIM)
	{
		if (bAimingAtABloodPool)
		{
			if (SpellsInventory::GetInstance() && SpellCheck::GetInstance() && conjuror)
			{
				currentBloodSpell = SpellsInventory::GetInstance()->GetBloodSpell(index_, corpseSpell); 

				switch (currentBloodSpell) //Note: We need the spell check inside the switch statement to prevent BP from dropping if the player uses a corpse spell on a blood pool (not on a corpse)
					{
					case EBloodSpells::BLOODMIASMA:
						if (SpellCheck::GetInstance()->CheckForBloodSpell(currentBloodSpell, stats->GetBP(), usingASpellSummon, spellBaseDamage, effect))
						{
							animInstance->buseBloodSpell1 = true;
							bCastingSpell = true;
						}
							break;
					case EBloodSpells::BLOODTORNADO:
						if (SpellCheck::GetInstance()->CheckForBloodSpell(currentBloodSpell, stats->GetBP(), usingASpellSummon, spellBaseDamage, effect))
						{
							animInstance->buseSpell2 = true;
							bCastingSpell = true;
						}
							break;
					case EBloodSpells::SUMMONSKELETON:
						if (SpellCheck::GetInstance()->CheckForBloodSpell(currentBloodSpell, stats->GetBP(), usingASpellSummon, spellBaseDamage, effect))
						{
							animInstance->buseBloodSpell2 = true;
							bCastingSpell = true;
						}
							break;
					default:
							bCastingSpell = false;
							return; //Fixes a bug where you're aiming at a BP and using a corpse-based spell
						}
			}
		}
		else if (bAimingAtEnemy) //Check if we're aiming at a corpse
		{
			if (SpellsInventory::GetInstance() && SpellCheck::GetInstance() && conjuror)
			{
				currentBloodSpell = SpellsInventory::GetInstance()->GetBloodSpell(index_, corpseSpell); //Check if we can use the spell

				if (SpellCheck::GetInstance()->CheckForBloodSpell(currentBloodSpell, stats->GetBP(), usingASpellSummon, spellBaseDamage, effect))
				{
					bCastingSpell = true;
					UE_LOG(LogTemp, Warning, TEXT("SPELL CHECK SUCCESS"));
					if (!corpseSpell)
					{
						switch (currentBloodSpell) //Yes, you can use blood spells using enemy bodies
						{
						case EBloodSpells::BLOODMIASMA:
							animInstance->buseBloodSpell1 = true;
							break;
						case EBloodSpells::BLOODTORNADO:
							animInstance->buseSpell2 = true;
							break;
						case EBloodSpells::SUMMONSKELETON:
							animInstance->buseBloodSpell2 = true;
							break;
						default:
							break;
						}
					}
					else
					{
						switch (currentBloodSpell)
						{
						case EBloodSpells::SERVEINDEATH:
							animInstance->buseBloodSpell2 = true;
							break;
						case EBloodSpells::BLOODEXPLOSION:
							animInstance->buseSpell2 = true;
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}
}

void ANecromancerCharacter::ConjurBloodSpell()
{
	if (currentBloodSpell != EBloodSpells::BLOODNONE)
	{
		if(magicCircle && !corpseSpell) //Spawn magic circle on blood pool
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), magicCircle, bloodSpellLocation, FRotator::ZeroRotator, FVector(1.0f, 1.0f, 1.0f));

		bloodSpellLocation.Z += 200.0f; //The spell should be conjured above the magic circle
		
		conjuror->ConjurBloodSpell(currentBloodSpell, bloodSpellLocation, GetViewRotation(), usingASpellSummon, stats->GetHP(), spellBaseDamage, effect, currentStatusDuration, playerController, cameraShake); //Conjur the spell
		
		if (corpseSpell) //If it's a corpse spell, check what it does
		{
			bloodSpellLocation.Z -= 230.0f;
			if (magicCircle) //Spawn magic circle under corpse
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), magicCircle, bloodSpellLocation, FRotator::ZeroRotator, FVector(1.0f, 1.0f, 1.0f));

			switch (currentBloodSpell)
			{
			case EBloodSpells::SERVEINDEATH:
				if (enemy)
					if(!enemy->bZombie) //Don't zombify a zombie
						enemy->Zombify();
				break;
			case EBloodSpells::BLOODEXPLOSION:
				if (enemy)
					enemy->Explode();
				break;
			default:
				break;
			}
		}
		else
		{
			if (enemy)
				enemy->Destroy();
		}

		currentBloodSpell = EBloodSpells::BLOODNONE;

		if (interactable)
			interactable->Interact(); //Absorbs blood pool
	}
}
#pragma endregion

#pragma region CallAndConjurInnateSpell
void ANecromancerCharacter::CallInnateSpell()
{
	//No need to aim for innate spells
	bool spellDamagesPlayer = false;
	bool spellIncreasesHealth = false;
	float spellBaseDamage = 0.0f;
	EStatusEffects effect = EStatusEffects::NONE;
	currentInnateSpell = SpellsInventory::GetInstance()->GetInnateSpell();

	if (currentInnateSpell != EInnateSpells::INNATENONE)
	{
		if (SpellCheck::GetInstance()->CheckForInnateSpell(currentInnateSpell, stats->GetBP(), spellDamagesPlayer, stats->GetHP(), increaseHP, spellBaseDamage, effect))
		{
			//TODO
			//Add magic stat to spellBaseDamage
			conjuror->ConjurInnateSpell(currentInnateSpell, GetActorLocation(), effect, currentStatusDuration);
		}
		else //Spell has either failed or heals/damages player
		{
			if (spellIncreasesHealth || spellDamagesPlayer)
			{

			}
			else
			{
				//TODO
				//Play error sound
			}
		}
	}
}
#pragma endregion

#pragma region UI
UPlayerUIController* ANecromancerCharacter::GetUIController() const
{
	return uictrl;
}
#pragma endregion

#pragma region CalculateDamage
void ANecromancerCharacter::TakeDamage(float damage_)
{
	if (currentState != EPlayerState::DASH)
	{
		if(cameraShake && playerController)
			playerController->PlayerCameraManager->PlayCameraShake(cameraShake, 0.5f);
		stats->GetHP() -= damage_;
		animInstance->bHit = true;
		UE_LOG(LogTemp, Warning, TEXT("Player has taken damage %f"), damage_);

		if (currentState != EPlayerState::HIT)
		{
			prevState = currentState;
			currentState = EPlayerState::HIT;
		}
		if (stats->GetHP() <= 0.5f)
		{
			stats->GetHP() = 0.0f;
			//Death animation
			//Death screen
			
			//Placeholder code
			//TODO
			//Should call a death function that changes the UI
			//Destroy();
			ANecromancerGameMode* gameMode = Cast<ANecromancerGameMode>(GetWorld()->GetAuthGameMode());
			if (gameMode)
				gameMode->RespawnPlayer();
		}
	}
}


void ANecromancerCharacter::EndHit()
{
	if (currentState == EPlayerState::HIT)
		currentState = prevState;
}
#pragma endregion



#pragma region Dash
//Dash
void ANecromancerCharacter::Dash()
{
	if (GetCharacterMovement()->Velocity.Size() > 0.0f && currentState != EPlayerState::DASH)
	{
		//Call Animation which will activate a particle system
		animInstance->bDash = true;
		currentState = EPlayerState::DASH;
	}
}

void ANecromancerCharacter::MoveDueToDash()
{
	dashVec = GetActorForwardVector()*dashDistance;//

	LaunchCharacter(dashVec,true,false);
	GetWorld()->GetTimerManager().SetTimer(dashTimeHandler, this, &ANecromancerCharacter::EndDash, dashTime, false);
	//We need to use a timer here because if a player keeps pressign shift to dash, there's a chance they might press it when in between the transition from the dash animation to the walking animation.

}

void ANecromancerCharacter::EndDash()
{
	if(!bAim) //If the player leaves the dash, to aim immediately, the state should not turn to idle
	currentState = EPlayerState::IDLE;
}

#pragma endregion

#pragma region InputSpellFunctions

void ANecromancerCharacter::CallAimSpell0()
{
	CallAimSpell(0);
}
void ANecromancerCharacter::CallAimSpell1()
{
	CallAimSpell(1);
}
void ANecromancerCharacter::CallAimSpell2()
{
	CallAimSpell(2);
}
void ANecromancerCharacter::CallAimSpell3()
{
	CallAimSpell(3);
}

void ANecromancerCharacter::CallBloodSpell0()
{
	CallBloodSpell(0);
}
void ANecromancerCharacter::CallBloodSpell1()
{
	CallBloodSpell(1);
}
void ANecromancerCharacter::CallBloodSpell2()
{
	CallBloodSpell(2);
}
void ANecromancerCharacter::CallBloodSpell3()
{
	CallBloodSpell(3);
}


#pragma endregion

#pragma region Animation functions

void ANecromancerCharacter::AnimShoot()
{
	if(currentState == EPlayerState::AIM)
	animInstance->banimAimShoot = true;
}

void ANecromancerCharacter::AnimShootProjectile()
{
	if (currentState == EPlayerState::AIM)
	{
		//FVector spawnPos = projectileSummonLocation->GetComponentLocation();
		//FRotator spawnRot = GetViewRotation();
		AAimProjectile* proj = GetWorld()->SpawnActor<AAimProjectile>(projectile, projectileSummonLocation->GetComponentLocation(), GetViewRotation());
		if (proj)
		{
			proj->SetDamage(stats->GetBaseDamage());
			proj->SetControllerAndCameraShake(playerController, cameraShake);
		}
	}
}

void ANecromancerCharacter::SpawnWall()
{
	if (bAimingAtABloodPool)
	{
		if (interactable)
		{
			interactable->WallAction(GetActorRightVector().Rotation()); //If aiming at a blood pool, spawn a wall. If aiming at a wall, destroy the wall
		}
	}
}
void ANecromancerCharacter::PlayWallAnimation()
{
	if (bAimingAtABloodPool)
	{
		if (interactable)
		{
			if (animInstance)
			{
				animInstance->bwall = true;
				bCastingSpell = true;
			}
		}
	}
}

#pragma endregion


#pragma region Dialogue
void ANecromancerCharacter::InitiateAutoDialogue()
{
	if (dialogueHandle)
		dialogueHandle->ProcessInput();
}
#pragma endregion

