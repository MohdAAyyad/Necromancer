// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "NecromancerCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
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
	currentState = EPlayerState::IDLE;
	hud = nullptr;
	playerController = nullptr;
	animInstance = nullptr;
	colParams = FCollisionQueryParams::DefaultQueryParam;
	interactable = nullptr;
	baimingAtABloodPool = false;

	projectileSummonLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Summon Location"));
	projectileSummonLocation->SetupAttachment(RootComponent,TEXT("RightHandSocket"));

	aimPartilces = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Aim Particle System"));
	aimPartilces->SetupAttachment(projectileSummonLocation);

	conjuror = CreateDefaultSubobject<USpellConjuror>(TEXT("Conjuror"));

	uictrl = CreateDefaultSubobject<UPlayerUIController>(TEXT("UI Controller"));

	//Stats
	maxHP = 100.0f;
	hp = 100.0f;
	maxBP = 100.0f;
	bp = 100.0f;
    lineCastLength = 1000.0f;

	currentAimSpell = EAimSpells::AIMNONE;
	currentBloodSpell = EBloodSpells::BLOODNONE;
	currentInnateSpell = EInnateSpells::INNATENONE;
	currentStatusEffect = EStatusEffects::NONE; //Out of these 5, this is the only one that marks the status effect of the player
	currentStatusDuration = EStatusDuration::MIN;
}

void ANecromancerCharacter::BeginPlay()
{
	Super::BeginPlay();
	playerController = GetWorld()->GetFirstPlayerController();
	hud = Cast<APlayerHUD>(playerController->GetHUD());

	if (aimPartilces)
	{
		aimPartilces->DeactivateSystem();
	}

	animInstance = Cast<ULeahAnimInstance>(GetMesh()->GetAnimInstance());
	SpellsInventory::GetInstance(); //Initiate instance

	uictrl->SetHealthPercentage(1.0f);
	uictrl->SetBloodPercentage(1.0f);
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

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ANecromancerCharacter::Shoot);

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
					if (!baimingAtABloodPool)
						baimingAtABloodPool = true;
				}
				else
				{
					hud->aimingAtEnemy();
					if (baimingAtABloodPool)
						baimingAtABloodPool = false;
				}
			}
			else
			{
				//If aiming at an actor that doesn't cast correctly, reset
				hud->aimingAtIdle();
				baimingAtABloodPool = false;
			}
		}
		else
		{ 
			//If aiming at nothing, reset
			hud->aimingAtIdle();
			baimingAtABloodPool = false;
			interactable = nullptr;
		}
	}
}

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
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ANecromancerCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
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

//Custom

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
		bUseControllerRotationYaw = false;
		aimPartilces->DeactivateSystem();
		if (animInstance)
			animInstance->baimMode = false;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		interactable = nullptr;  //Reset interactable
		baimingAtABloodPool = false; //Reset baimingAtABloodPool
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
			animInstance->baimMode = true;
		GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	}
}


void ANecromancerCharacter::Shoot()
{
	if (currentState == EPlayerState::AIM)
	{
		FVector spawnPos = projectileSummonLocation->GetComponentLocation();
		FRotator spawnRot = GetViewRotation();
		GetWorld()->SpawnActor<AAimProjectile>(projectile, spawnPos, spawnRot);
	}
}

void ANecromancerCharacter::AimInteract() //Pressing interact
{
	if (currentState == EPlayerState::AIM)
	{
		if (interactable && baimingAtABloodPool)
		{
			interactable->Interact(bp,true); //absorb blood points from the blood pool

			if (bp >= maxBP)
			{
				bp = maxBP;
				uictrl->SetBloodPercentage(1.0f);
			}
			else
			{
				uictrl->SetBloodPercentage(bp / maxBP);
			}
			UE_LOG(LogTemp, Warning, TEXT("New BPs = %f"), bp);
		}
	}
}


void ANecromancerCharacter::AimInteractHeal() //Pressing interact heal
{
	if (currentState == EPlayerState::AIM)
	{
		if (interactable && baimingAtABloodPool)
		{
			interactable->Interact(hp, false); //absorb blood points from the blood pool

			if (hp >= maxHP)
			{
				hp = maxHP;
				uictrl->SetHealthPercentage(1.0f);
			}
			else
			{
				uictrl->SetHealthPercentage(hp/maxHP);
			}
			UE_LOG(LogTemp, Warning, TEXT("New HPs = %f"), hp);

		}
	}
}

void ANecromancerCharacter::CallAimSpell(int index_)
{
	if (currentState == EPlayerState::AIM)
	{
		bool increaseHP = false;
		float spellBaseDamage = 0.0f;
		EStatusEffects effect = EStatusEffects::NONE;
		if (SpellsInventory::GetInstance() && conjuror)
		{
			currentAimSpell = SpellsInventory::GetInstance()->UseAimSpell(index_, bp, increaseHP, spellBaseDamage, effect); //Check if we can use the spell
			//TODO
			//Add magic stat to spellBaseDamage

			if (currentAimSpell != EAimSpells::AIMNONE)
			{
				conjuror->ConjourAimSpell(currentAimSpell, GetActorLocation(), increaseHP, hp, spellBaseDamage, effect, currentStatusDuration); //Conjur the spell
				uictrl->SetBloodPercentage(bp / maxBP);
				if (increaseHP) //Only if we increased our health, update UI.
				{
					uictrl->SetHealthPercentage(hp / maxHP);
				}
			}
		}
	}
}
void ANecromancerCharacter::CallBloodSpell(int index_)
{

}
void ANecromancerCharacter::CallInnateSpell()
{

}

UPlayerUIController* ANecromancerCharacter::GetUIController() const
{
	return uictrl;
}

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
