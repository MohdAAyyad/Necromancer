// Fill out your copyright notice in the Description page of Project Settings.

//TODO
//Cast 0 should have its collider enabled when the spikes appear not before
//Special cast
//Geomancer needs to evade when getting hit a lot or when the player is too close
//Zombification
//TODO

#include "Geomancer.h"
#include "GeoAnimInstance.h"
#include "Components/CapsuleComponent.h"

AGeomancer::AGeomancer() :AKnight()
{
	acceptableAttackDistance = 1000.0f;
	hp = 150.0f;
	reloadTime = 1.0f;
	animInstance = nullptr;
	currentState = EGeomancerState::SITTING;
	permenantTarget = nullptr;
	bHasChosenSpell = false;
	evadeRange = 500.0f;
	evadeDestination = FVector::ZeroVector;
	bhasChosenEvadeDestination = false;

	castSpell1Location = CreateDefaultSubobject<USceneComponent>(TEXT("Spell1 Cast Loc"));
	castSpell1Location->SetupAttachment(RootComponent);
	evadeParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Evade Particles"));
	evadeParticles->SetupAttachment(RootComponent);
}

void AGeomancer::BeginPlay()
{
	AEnemyBase::BeginPlay();
	animInstance = Cast<UGeoAnimInstance>(GetMesh()->GetAnimInstance());
	if (sense)
	{
		sense->OnSeePawn.AddDynamic(this, &AGeomancer::OnSeePlayer);		
	}

	if (currentState == EGeomancerState::SITTING)
	{
		//If the enemy is sitting, make sure the animation state starts at sitting
		if (animInstance)
			animInstance->SetSitting();
		if (aiController)
			aiController->SetSitting(true);
	}

	if (evadeParticles)
		evadeParticles->DeactivateSystem();
}
void AGeomancer::Tick(float DeltaTime)
{
	AEnemyBase::Tick(DeltaTime);

	if (permenantTarget) //Only work when you actually see a target
	{
		if (currentState == EGeomancerState::EVADING)
		{
			//Teleports to another location
			if (!bhasChosenEvadeDestination)
			{
				//Always teleport behind the player
				evadeDestination = permenantTarget->GetActorLocation() - permenantTarget->GetActorForwardVector()*evadeRange;
				bhasChosenEvadeDestination = true;
			}
			else
			{
				//Move to the new location, and go reset the state
				SetActorLocation(evadeDestination);
				currentState = EGeomancerState::PATROLLING;
				if (animInstance)
					animInstance->ResetEvading();
				if (evadeParticles)
					evadeParticles->ActivateSystem(true);
				if (aiController)
				{
					//After evading, reset the BT
					aiController->SetEvading(false);
					aiController->SetCast(false);
					aiController->SetReload(false);
					aiController->SetHit(false);
					aiController->SetSitting(false);
				}
				bHasChosenSpell = false; //In case the enemy was casting a spell when it got hit, reset the spell 
			}

		}
		else if (currentState == EGeomancerState::SITTING)
		{
			if (animInstance)
				animInstance->ResetSitting();
		}
		else if (currentState == EGeomancerState::PATROLLING || currentState == EGeomancerState::TOPLAYER || bZombie) //If you find the player while patrolling, or if you see an enemy while a zombie
		{
			//UE_LOG(LogTemp, Error, TEXT("Inside Patrolling"));
			float distance = GetDistanceToPlayer();

			if (distance > acceptableAttackDistance) //If the target is far, go to acceptable attack distance
			{
				//UE_LOG(LogTemp, Error, TEXT("To player dude"));
				currentState = EGeomancerState::TOPLAYER;
				if (aiController)
				{
					aiController->SetToPlayer(true);
					aiController->SetCast(false);
				}
			}
			else //If the player is close enough for you to attack, then attack
			{
				if (!bHasChosenSpell)
				{
					currentState = EGeomancerState::ATTACKING;
					if (aiController)
					{
						aiController->SetCast(true);
						aiController->SetToPlayer(false);
					}
					Attack();
					bHasChosenSpell = true;
				}
			}
		}
	}
	else if (currentState == EGeomancerState::PATROLLING) //Have not seen the target at all, and is patrolling
	{
		Patrol();
	}
}
void AGeomancer::OnSeePlayer(APawn* pawn_)
{
	//UE_LOG(LogTemp, Warning, TEXT("Alive seen this %s"), *pawn_->GetName());
	if (!bDead || bZombie)
	{
		WhoToLookFor(pawn_); //Look for player or enemy?

		if (!permenantTarget && !bZombie) //If we're a zombie, permenant target will be filled from inside who to look for
			permenantTarget = pawn_;
		else if (!permenantTarget && bZombie)
			permenantTarget = enemyForZombie;
	}
}
void AGeomancer::Attack()
{
	int attackChance = FMath::RandRange(0, 89);

	if (attackChance >= 0 && attackChance <= 29 && prevChosenSpell > 29)
	{
		if (FMath::Abs(permenantTarget->GetActorLocation().Z - GetActorLocation().Z) <= 30.0f) //This is a ground based attck. If the player is on high ground, it should not be initiated
		{
			if (animInstance)
				animInstance->NextCast();

			prevChosenSpell = attackChance;
		}
		else
		{
			Attack();
		}
	}
	if (attackChance >= 30 && attackChance <= 59 && (prevChosenSpell > 59 || prevChosenSpell < 30))
	{
		if (animInstance)
			animInstance->NextCast1();

		prevChosenSpell = attackChance;
	}
	else if (attackChance >= 60 && attackChance <= 89 && prevChosenSpell < 60)
	{
		if (animInstance)
			animInstance->NextSpecial();

		prevChosenSpell = attackChance;
	}
	else
	{
		Attack();
	}

	//The reason why prevChosenSpell is not updated at the end instead of updating inside each if statement is
	//to prevent the scenario of chose spell 2 then 1 but the player is on high ground, ok choose 2 again

}
void AGeomancer::EndAttack() //Called from anim instance after cast animation is done
{
	bHasChosenSpell = false;

	currentState = EGeomancerState::RELOADING;
	GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &AGeomancer::EndReload, reloadTime, false);

	if (aiController)
	{
		aiController->SetCast(false);
		aiController->SetReload(true);
	}
}

void AGeomancer::EndReload()
{
	aiController->SetReload(false);
	aiController->SetHit(false);

	if (animInstance)
		animInstance->ResetReload();

	currentState = EGeomancerState::PATROLLING;
}
void AGeomancer::Death()
{
	//Enemy die animation
	if (animInstance)
		animInstance->Death();
	bDead = true;
	aiController->SetSeenTarget(nullptr);
	permenantTarget = nullptr;
	aiController->SetDead();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (!hasAddedEXP)
		AddEXP();
}
void AGeomancer::Zombify()
{
	bZombie = true;
	if (animInstance)
		animInstance->SetZombify();
	GetWorld()->GetTimerManager().SetTimer(zombifyTimerHandle, this, &AGeomancer::EndZombify, zombifyDuration, false);
	hp = 70.0f;
	AEnemyBase::OnSeePlayer(nullptr);
}
void AGeomancer::EndZombify()
{
	currentState = EGeomancerState::DEATH;
	if (animInstance)
		animInstance->Death();
}

void AGeomancer::ActivateZombie()
{
	if (aiController)
	{
		aiController->ResetDead();
	}
	currentState = EGeomancerState::PATROLLING;
	sense->bOnlySensePlayers = false;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionProfileName("Zombie");
}

void AGeomancer::SpawnCastProjectile()
{
	//Summons spikes in the ground that rush towards the player
	if (projectiles[0])
	{
		FRotator spawnRot = (aiController->GetPlayerLocation() - GetActorLocation()).Rotation();

		AEnemyProjectile* proj = GetWorld()->SpawnActor<AEnemyProjectile>(projectiles[0], castSpellLocation->GetComponentLocation(), spawnRot);

		if (bZombie && proj)
		{
			proj->ChangeProfileName("ZombieProjectile");
		}
	}
}
void AGeomancer::SpawnCastProjectile1()
{
	UE_LOG(LogTemp, Warning, TEXT("Spanw Cast Proj 1"));
	//TODO
	if (projectiles[1])
	{
		FRotator spawnRot = (aiController->GetPlayerLocation() - GetActorLocation()).Rotation();

		AEnemyProjectile* proj = GetWorld()->SpawnActor<AEnemyProjectile>(projectiles[1], castSpell1Location->GetComponentLocation(), spawnRot);

		if (bZombie && proj)
		{
			proj->ChangeProfileName("ZombieProjectile");
		}
	}
}
void AGeomancer::SpawnSpecialProjectile()
{
	UE_LOG(LogTemp, Warning, TEXT("Spanw Cast Special"));
	//TODO
	if (projectiles[2])
	{
		if (permenantTarget)
		{
			FVector summonLoc = permenantTarget->GetActorLocation();
			summonLoc.Z -= 100.0f;
			AEnemyProjectile* proj = GetWorld()->SpawnActor<AEnemyProjectile>(projectiles[2], summonLoc, FRotator::ZeroRotator);

			if (bZombie && proj)
			{
				proj->ChangeProfileName("ZombieProjectile");
			}
		}
	}
}

void AGeomancer::TakeRegularDamage(float damage_)
{
	if (!bDead)
	{
		hp -= damage_;
		//UE_LOG(LogTemp, Warning, TEXT("HP is %f"), hp);

		if (hp <= 0.5f)
		{
			bTypeOfBPToSpawn = false;
			Death();
		}
		else
		{
			if (animInstance && currentState != EGeomancerState::ATTACKING)
				animInstance->SetHit();

			CalculateEvade();
		}
		
	}

	if (!aiController->GetPlayer() && !bZombie)
	{
		if(currentState==EGeomancerState::SITTING) //If you're sitting, stand up
			if (animInstance)
				animInstance->ResetSitting();

		OnSeePlayer(GetWorld()->GetFirstPlayerController()->GetPawn()); //If the enemy has not seen the player yet, and the player attacks it, it should now see the player
	}
}
void AGeomancer::TakeSpellDamage(float damage_)
{
	if (!bDead)
	{
		hp -= damage_;
		//UE_LOG(LogTemp, Warning, TEXT("HP is %f"), hp);

		if (hp <= 0.5f)
		{
			bTypeOfBPToSpawn = true;
			Death();
		}
		else
		{
			if (animInstance && currentState != EGeomancerState::ATTACKING)
				animInstance->SetHit();

			CalculateEvade();
		}
	}

	if (!aiController->GetPlayer() && !bZombie)
	{
		if (currentState == EGeomancerState::SITTING) //If you're sitting, stand up
			if (animInstance)
				animInstance->ResetSitting();

		OnSeePlayer(GetWorld()->GetFirstPlayerController()->GetPawn()); //If the enemy has not seen the player yet, and the player attacks it, it should now see the player
	}
}
void AGeomancer::TakeSpellDamage(float damage_, EStatusEffects effect_, float duration_)
{
	if (!bDead)
	{
		hp -= damage_;
		//UE_LOG(LogTemp, Warning, TEXT("HP is %f"), hp);

		if (hp <= 0.5f)
		{
			bTypeOfBPToSpawn = true;
			Death();
		}
		else
		{
			if (effect_ != EStatusEffects::NONE && currentStatusEffect == EStatusEffects::NONE)
			{
				currentStatusEffect = effect_;
				statusEffectDuration = duration_;
				ApplyStatusEffect();
			}

			if (animInstance && currentState != EGeomancerState::ATTACKING)
				animInstance->SetHit();

			CalculateEvade();
		}
	}

	if (!aiController->GetPlayer() && !bZombie)
	{
		OnSeePlayer(GetWorld()->GetFirstPlayerController()->GetPawn()); //If the enemy has not seen the player yet, and the player attacks it, it should now see the player
	}
}

void AGeomancer::CalculateEvade()
{
	int evadeChance = FMath::RandRange(0, 100);

	if (evadeChance <= 50) //50% chance to evade
	{
		if (animInstance)
			animInstance->SetEvading();
		if (aiController)
			aiController->SetEvading(true);
		bhasChosenEvadeDestination = false;
	}
}

void AGeomancer::EvadeTeleport()
{
	currentState = EGeomancerState::EVADING;
}

void AGeomancer::EndSitting()
{
	currentState = EGeomancerState::PATROLLING;
	if (aiController)
		aiController->SetSitting(false);
}

float AGeomancer::GetDistanceToPlayer()
{
	if (permenantTarget)
		return ((permenantTarget->GetActorLocation() - GetActorLocation()).Size());
	else
		return 10000.0f; //
}