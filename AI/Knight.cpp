// Fill out your copyright notice in the Description page of Project Settings.


#include "Knight.h"
#include "NecromancerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "KnightAnimInstance.h"

//TODO

// Solve the bug where if you hit the knight when it still doesn't see you, it breaks


AKnight::AKnight():AEnemyBase()
{
	 hp = 150.0f;
	 maxHP = 150.0f;
	 baseDamage = 15.0f;
	 damageModifier = 1.0f; //This increases the higher the player's level is or due to special spells
	 bDead = false;
	 animInstance = nullptr;
	 currentState = EKnightState::PATROLLING;
	 hasPickedApatrolDestination = false;
	 moveLoc = FVector::ZeroVector;
	 acceptableStrafeDistance = 50.0f;
	 acceptableCastDistance = 600.0f;
	 acceptableMeleeDistance = 100.0f;
	 acceptableAttackDistance = acceptableCastDistance + acceptableMeleeDistance;
	 hasUsedCastFireBall = false;
	 PrimaryActorTick.bCanEverTick = true;
	 reloadTime = 1.7f;
	 specialModifier = 0.5f;
	 hasCastSpecial = false;

	 attackHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack Hit Box"));
	 attackHitBox->SetCollisionProfileName("OverlapAllDynamic");
	 attackHitBox->SetBoxExtent(FVector(20.0f, 20.0f, 20.0f), true);
	 attackHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	 attackHitBox->SetGenerateOverlapEvents(true);
	 attackHitBox->SetupAttachment(RootComponent);

	 timeToDisableAttackHitBox = 0.3f;


	 GetCharacterMovement()->bOrientRotationToMovement = true;
	 GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 540.0f);

	 specialParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("specialParticles"));
	 specialParticles->SetupAttachment(RootComponent);

	 castSpellLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Spell Cast Loc"));
	 castSpellLocation->SetupAttachment(RootComponent);	

	 GetCapsuleComponent()->SetCollisionProfileName("AliveEnemy");
}

void AKnight::BeginPlay()
{
	Super::BeginPlay();
	animInstance = Cast<UKnightAnimInstance>(GetMesh()->GetAnimInstance());
	if (sense)
	{
		sense->OnSeePawn.AddDynamic(this, &AKnight::OnSeePlayer);
	}

	attackHitBox->OnComponentBeginOverlap.AddDynamic(this, &AKnight::AttackOverlap);
	attackHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (specialParticles)
		specialParticles->DeactivateSystem();

}

void AKnight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("Knight State is %d"), currentState);
	switch (currentState)
	{
	case AKnight::EKnightState::PATROLLING:
		Patrol();
		break;
	case AKnight::EKnightState::STRAFING:
		Strafe();
		break;
	case AKnight::EKnightState::TOPLAYER:
		ToPlayer();
		break;
	default:
		break;
	}

}
#pragma region CalculateDamage

void AKnight::TakeRegularDamage(float damage_)
{
	if (!bDead)
	{
		hp -= damage_;
		UE_LOG(LogTemp, Warning, TEXT("damage is %f"), damage_);

		if (hp <= 0.5f)
		{
			bTypeOfBPToSpawn = false;
			Death();
		}
		else
		{
			if (hp > 0.4f*maxHP && !hasCastSpecial)
			{
				if (currentState != EKnightState::CASTING && currentState != EKnightState::ATTACKING && currentState != EKnightState::DEATH)
					if (animInstance)
						animInstance->SetHit();
				if (aiController)
					aiController->SetHit(true);
			}
			else
			{
				CheckForSpecial();
			}
		}

	}

	if (!aiController->GetPlayer() && !bZombie)
	{
		moveLoc = GetActorLocation() - 75.0f*GetActorRightVector();
		OnSeePlayer(GetWorld()->GetFirstPlayerController()->GetPawn()); //If the enemy has not seen the player yet, and the player attacks it, it should now see the player
		currentState = EKnightState::PATROLLING;
	}
}

void AKnight::TakeSpellDamage(float damage_, EStatusEffects effect_, float duration_)
{
	if (!bDead)
	{
		hp -= damage_;
		UE_LOG(LogTemp, Warning, TEXT("damage is %f"), damage_);

		if (hp <= 0.5f)
		{
			bTypeOfBPToSpawn = true; //True-->Tainted
			Death();
			
		}
		else
		{
			//Check if the new status effect is applicable
			if (effect_ != EStatusEffects::NONE && currentStatusEffect == EStatusEffects::NONE)
			{
				//UE_LOG(LogTemp, Warning, TEXT("POISONED!"));
				currentStatusEffect = effect_;
				statusEffectDuration = duration_;
				ApplyStatusEffect();
			}

			if (hp > 0.4f*maxHP)
			{
				if (currentState != EKnightState::CASTING && currentState != EKnightState::ATTACKING && currentState != EKnightState::DEATH)
					if (animInstance)
						animInstance->SetHit();
				if (aiController)
					aiController->SetHit(true);
			}
			else
			{
				CheckForSpecial();
			}
		}

	}

	if (!aiController->GetPlayer())
	{
		moveLoc = GetActorLocation() - 75.0f*GetActorRightVector();
		OnSeePlayer(GetWorld()->GetFirstPlayerController()->GetPawn()); //If the enemy has not seen the player yet, and the player attacks it, it should now see the player
		currentState = EKnightState::PATROLLING;
	}
}

void AKnight::TakeSpellDamage(float damage_)
{
	if (!bDead)
	{
		hp -= damage_;
		UE_LOG(LogTemp, Warning, TEXT("damage is %f"), damage_);

		if (hp <= 0.5f)
		{
			bTypeOfBPToSpawn = true; //True-->Tainted
			Death();

		}
		else
		{
			if (hp > 0.4f*maxHP)
			{
				if (currentState != EKnightState::CASTING && currentState != EKnightState::ATTACKING && currentState != EKnightState::DEATH)
					if (animInstance)
						animInstance->SetHit();
				if (aiController)
					aiController->SetHit(true);
			}
			else
			{
				CheckForSpecial();
			}
		}

	}

	if (!aiController->GetPlayer())
	{
		moveLoc = GetActorLocation() - 75.0f*GetActorRightVector();
		OnSeePlayer(GetWorld()->GetFirstPlayerController()->GetPawn()); //If the enemy has not seen the player yet, and the player attacks it, it should now see the player
		currentState = EKnightState::PATROLLING;
	}
}

//Called inside take damage functions to see if health is less than 60% and if it is, use special once
void AKnight::CheckForSpecial()
{
	if (!hasCastSpecial)
	{
		if (animInstance)
		{
			animInstance->ResetStrafe();
			animInstance->ResetAttack();
			animInstance->ResetCast();
			animInstance->ResetReload();
			animInstance->ResetHit();
			animInstance->NextSpecial();
		}

		if (aiController)
		{
			aiController->SetSpecial(true);
			aiController->SetAttack(false);
			aiController->SetReload(true);
		}

		if (specialParticles)
			specialParticles->ActivateSystem(true);
		damageModifier += specialModifier;
		hp += 50.0f; //Regain some health
		GetCharacterMovement()->MaxWalkSpeed *= 2.5f; //Increase speed
		hasCastSpecial = true;
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
		currentState = EKnightState::SPECIAL; //Casting special
	}
}

void AKnight::Death()
{
	//Enemy die animation
	if (animInstance)
		animInstance->Death();


	bDead = true;
	aiController->SetSeenTarget(nullptr);
	aiController->SetDead();
	currentState = EKnightState::DEATH;
	if (specialParticles)
		specialParticles->DeactivateSystem();
	attackHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (!hasAddedEXP)
		AddEXP();
}

#pragma endregion

#pragma region Zombification

void AKnight::Zombify()
{//Called from the player script when zombify spell is used
	if (animInstance)
		animInstance->SetZombify();
	//Kill the zombie after the zombify duration has passed
	GetWorld()->GetTimerManager().SetTimer(zombifyTimerHandle, this, &AKnight::EndZombify, zombifyDuration, false);
	hasCastSpecial = false;
	hp = 10.0f;
	Super::OnSeePlayer(nullptr);
	bZombie = true;
}

void AKnight::EndZombify()
{

	currentState = EKnightState::DEATH;
	if (animInstance)
		animInstance->Death();
}

void AKnight::ActivateZombie()
{
	//Called from animation
	sense->bOnlySensePlayers = false; //See other enemies as well
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionProfileName("Zombie");
	attackHitBox->SetCollisionProfileName("ZombieProjectile");
	if (aiController)
	{
		aiController->ResetDead();
		//Don't move once you turn into a zombie
		aiController->SetNewLocation(GetActorLocation());
	}
	//currentState = EKnightState::TOPLAYER;
}
#pragma endregion


#pragma region See Player

void AKnight::OnSeePlayer(APawn* pawn_)
{
	//UE_LOG(LogTemp, Warning, TEXT("Alive seen this %s"), *pawn_->GetName());
	if (!bDead || bZombie)
	{
		WhoToLookFor(pawn_);

		if (aiController->GetPlayer()) //Only work when you actually see a target
		{
			if ((currentState == EKnightState::PATROLLING || bZombie) && !hasCastSpecial) //If special is cast, go for melee
			{
				//UE_LOG(LogTemp, Error, TEXT("Inside Patrolling"));
				float distance = GetDistanceToPlayer();

				if (distance <= acceptableAttackDistance) //If the player is close enough, go to player
				{
					currentState = EKnightState::TOPLAYER;
				}

				else if (distance >= acceptableStrafeDistance) //If the player is far enough, strafe, if they're close, attack. //Special doesn't strafe
				{
					//UE_LOG(LogTemp, Error, TEXT("Strafe dude"));
					currentState = EKnightState::STRAFING;
					moveLoc = GetActorLocation() - 75.0f*GetActorRightVector();

					//UE_LOG(LogTemp, Error, TEXT("Strafe Move Loc %f %f %f"),moveLoc.X, moveLoc.Y, moveLoc.Z);
					if (aiController)
					{
						aiController->SetNewLocation(moveLoc); //Pass in the target location for the move to node in BT
						aiController->SetStrafe(true);
						aiController->SetToPlayer(false);
					}
					if (animInstance)
						animInstance->SetStrafe();
				}
				else
				{
					//UE_LOG(LogTemp, Error, TEXT("To player dude"));
					currentState = EKnightState::TOPLAYER;
					if (aiController)
					{
						moveLoc = aiController->GetPlayerLocation();
						aiController->SetNewLocation(moveLoc); //Pass in the target location for the move to node in BT
						aiController->SetStrafe(false);
						aiController->SetToPlayer(true);
					}
				}
			}
			else if (hasCastSpecial && currentState != EKnightState::ATTACKING) //If you're attacking, don't decide what the next attack is until it finishes
			{
				currentState = EKnightState::TOPLAYER;
			}
		}
	}
}

#pragma endregion

void AKnight::Strafe()
{
		if ((moveLoc - GetActorLocation()).Size() <= 1.0f) // If you've reached the strafing destination, move to player
		{
			currentState = EKnightState::TOPLAYER;
			animInstance->ResetStrafe();
			if (aiController)
			{
				aiController->SetStrafe(false);
				aiController->SetToPlayer(true);
			}
			//UE_LOG(LogTemp, Warning, TEXT("Reset Strafe"));
		}
}

void AKnight::ToPlayer()
{
	float distance = GetDistanceToPlayer();
	int castChance = 0;

	//UE_LOG(LogTemp, Warning, TEXT("Inside ToPlayer"));
	if (GetDistanceToPlayer() <= acceptableMeleeDistance) //If you're close enough to the player, melee
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is close enough"));
		currentState = EKnightState::ATTACKING;
		if (aiController)
		{
			aiController->SetAttack(true);
			aiController->SetStrafe(false);
			aiController->SetToPlayer(false);
			aiController->SetReload(false);
		}

		animInstance->NextAttack();
		castChance = FMath::RandRange(0, 40); //See if you can cast again
		if (castChance <= 15)
			hasUsedCastFireBall = false;
	}
	else if (distance <= acceptableCastDistance && !hasCastSpecial && !hasUsedCastFireBall) //If the player is too far for melee but close enough for cast, cast
	{
	//UE_LOG(LogTemp, Error, TEXT("Cast Spell dude"));
	animInstance->NextCast();
	animInstance->ResetHit();
	aiController->SetCast(true);
	currentState = EKnightState::CASTING;
	hasUsedCastFireBall = true;
	}
	else if (distance > acceptableAttackDistance) //Otherwise just go to the player
	{
		aiController->SetToPlayer(true);
		aiController->SetAttack(false);
		aiController->SetStrafe(false);
		aiController->SetReload(false);
		aiController->SetCast(false);
	}
}


void AKnight::Reload()
{
	currentState = AKnight::EKnightState::RELOADING;
	attackHitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &AKnight::EndReload, reloadTime, false);

	if (animInstance)
	{
		animInstance->Reload();
		animInstance->ResetHit();
	}

	if (aiController)
	{
		aiController->SetAttack(false);
		aiController->SetStrafe(false);
		aiController->SetToPlayer(false);
		aiController->SetCast(false);
		aiController->SetReload(true);
	}
}


void AKnight::EndReload()
{	//Reset
	//UE_LOG(LogTemp, Warning, TEXT("Reload End"));
	aiController->SetReload(false);
	aiController->SetHit(false);
	if (!hasCastSpecial)
	{
		currentState = EKnightState::PATROLLING;
		OnSeePlayer(GetWorld()->GetFirstPlayerController()->GetPawn());
	}
	else
		currentState = EKnightState::TOPLAYER;

	if (animInstance)
	{
		//animInstance->ResetStrafe();
		//animInstance->ResetAttack();
		animInstance->ResetCast();
		animInstance->ResetReload();
	}

}


void AKnight::EnableHitBox()
{
	//Called from animInstance after anim notify
	attackHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AKnight::AttackOverlap (UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ != nullptr && otherActor_ != this && overlappedComponent_ != nullptr)
	{
		if (!bZombie)
		{
			ANecromancerCharacter* player = Cast<ANecromancerCharacter>(otherActor_);

			if (player)
			{
				player->TakeDamage(baseDamage * damageModifier);
			}
		}
		else
		{
			AEnemyBase* enemy = Cast<AEnemyBase>(otherActor_); //This is used for zombies
			if (enemy)
			{
				enemy->TakeSpellDamage(baseDamage * damageModifier);
			}
		}
	}
}

void AKnight::SpawnCastProjectile() //Called from animationInstance
{
	AEnemyProjectile* proj = GetWorld()->SpawnActor<AEnemyProjectile>(castProjectile, castSpellLocation->GetComponentLocation(), GetActorRotation());

	if (bZombie && proj)
	{
		proj->ChangeProfileName("ZombieProjectile");
	}
}

void AKnight::EndHit()
{
	if (aiController)
		aiController->SetHit(false);
}

void AKnight::EndSpecial()
{
	if (aiController)
	{
		aiController->SetSpecial(false);
		aiController->SetToPlayer(true);
		aiController->SetReload(false);
	}

	currentState = EKnightState::TOPLAYER;
}