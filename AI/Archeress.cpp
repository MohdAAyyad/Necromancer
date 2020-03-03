// Fill out your copyright notice in the Description page of Project Settings.


#include "Archeress.h"
#include "ArcheressAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "NecromancerCharacter.h"



AArcheress::AArcheress():AEnemyBase()
{
	hp = 100.0f;
	maxHP = 100.0f;
	baseDamage = 15.0f;
	damageModifier = 1.0f; //This increases the higher the player's level is
	bDead = false;
	currentState = EArcheressState::IDLE;
	drawTime = 0.5f;
	attackAcceptableDistance = 2000.0f;

	castChance = 0;

	castParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("castParticles"));
	castParticles->SetupAttachment(RootComponent);

	arrowSummonLocation = CreateDefaultSubobject<USceneComponent>(TEXT("arrowSummonLocation"));
	arrowSummonLocation->SetupAttachment(RootComponent);
	GetCapsuleComponent()->SetCollisionProfileName("AliveEnemy");
}
void AArcheress::BeginPlay()
{
	Super::BeginPlay();

	animInstance = Cast<UArcheressAnimInstance>(GetMesh()->GetAnimInstance());
	if (sense)
	{
		sense->OnSeePawn.AddDynamic(this, &AArcheress::OnSeePlayer);
	}

	if (castParticles)
		castParticles->DeactivateSystem();
}
void AArcheress::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region See Player

void AArcheress::OnSeePlayer(APawn* pawn_)
{
	if (!bDead || bZombie)
	{
		WhoToLookFor(pawn_);

		if (aiController->GetPlayer()) //Only work when you actually see a target
		{
			if (GetDistanceToPlayer() <= attackAcceptableDistance && currentState == EArcheressState::IDLE) //If the player is close enough, aim
			{
				if (animInstance)
				{
					if (!animInstance->IsMontagePlaying())
					{
						castChance = FMath::RandRange(0, 40);
					}
					if (castChance >= 0 && castChance <= 10)
					{
						animInstance->NextCast();
						if (castParticles)
							castParticles->ActivateSystem(true);
					}
					else
					{
						animInstance->NextAttack();
					}
					currentState = EArcheressState::AIM;
				}
			}
		}
	}
}


void AArcheress::WhoToLookFor(APawn* pawn_)
{
	if (!player && !bZombie) //If we have yet to see the player, and we're not a zombie, make sure you look for the player
	{
		//This is to prevent the enemy from going after other enemies when it's not a zombie
		player = Cast<ANecromancerCharacter>(pawn_);
		if (player)
			Super::OnSeePlayer(player);
	}
	else if (bZombie) //Otherwise look for other enemies
	{
		//UE_LOG(LogTemp, Warning, TEXT("Zombie seen this %s"), *pawn_->GetName());
		if (pawn_ != player)
		{
			enemyForZombie = Cast<AEnemyBase>(pawn_);
			if (enemyForZombie)
			{
				//UE_LOG(LogTemp,Warning,TEXT("Zombie has seen enemy"))
					if (!enemyForZombie->bZombie && !enemyForZombie->IsDead()) //Make sure you don't fight a fellow zombie
					{
						//UE_LOG(LogTemp, Warning, TEXT("Enemy is not zombie"))
						Super::OnSeePlayer(pawn_);
					}
			}
		}
	}
	else if (player)
	{
		Super::OnSeePlayer(player);
	}
}

#pragma endregion

void AArcheress::SpawnBloodPool()
{
	if (!bZombie)
	{
		if (!bTypeOfBPToSpawn) //Untainted false, tainted true
		{
			if (bloodPools[0])
			{
				//Summon blood pool starting at the torso
				//Code should run after death animation is run
				GetWorld()->SpawnActor<ABloodPool>(bloodPools[0], GetMesh()->GetBoneLocation("Hips", EBoneSpaces::WorldSpace), GetActorRotation());
			}
		}
		else
		{
			if (bloodPools[1])
			{
				//Summon blood pool starting at the torso
				//Code should run after death animation is run
				GetWorld()->SpawnActor<ABloodPool>(bloodPools[1], GetMesh()->GetBoneLocation("Hips", EBoneSpaces::WorldSpace), GetActorRotation());
			}
		}
	}
	else
	{
		Destroy();
	}

}

void AArcheress::TakeRegularDamage(float damage_)
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
			if (animInstance)
				animInstance->SetHit();
		}
	}
}
void AArcheress::TakeSpellDamage(float damage_, EStatusEffects effect_, float duration_)
{
	UE_LOG(LogTemp, Warning, TEXT("Taken spell damage %f"), damage_);
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

			if (animInstance)
				animInstance->SetHit();
		}
	}
}

void AArcheress::Death()
{
	//Enemy die animation
	if (animInstance)
		animInstance->Death();

	//TODO
	//Enable enemy to be targeted for "Serve in Death" spell

	bDead = true;
	aiController->SetSeenTarget(nullptr);
	currentState = EArcheressState::DEATH;
	if (castParticles)
		castParticles->DeactivateSystem();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
#pragma region Zombification

void AArcheress::Zombify()
{
	bZombie = true;
	if (animInstance)
		animInstance->SetZombify();
	GetWorld()->GetTimerManager().SetTimer(zombifyTimerHandle, this, &AArcheress::EndZombify, zombifyDuration, false);
	hp = 70.0f;
	Super::OnSeePlayer(nullptr);
}

void AArcheress::EndZombify()
{
	currentState = EArcheressState::DEATH;
	if (animInstance)
		animInstance->Death();
}

void AArcheress::ActivateZombie()
{
	if (aiController)
	{
		aiController->ResetDead();
	}
	currentState = EArcheressState::IDLE;
	sense->bOnlySensePlayers = false;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionProfileName("Zombie");
}

#pragma endregion

#pragma region Shoot Arrows

void AArcheress::ShootArrow()
{
	if (animInstance)
		animInstance->NextAttack();
}

void AArcheress::SpawnRegularArrow()
{
	FVector vec = aiController->GetPlayerLocation() - GetActorLocation();
	if (arrows[0])
	{
		AEnemyProjectile* proj = GetWorld()->SpawnActor<AEnemyProjectile>(arrows[0], arrowSummonLocation->GetComponentLocation(), vec.Rotation());
		if (bZombie && proj)
		{
			proj->ChangeProfileName("ZombieProjectile");
		}
	}

	currentState = EArcheressState::IDLE;
}

void AArcheress::DeactivateCastParticles()
{
	if (castParticles)
		castParticles->DeactivateSystem();

	FVector vec = aiController->GetPlayerLocation() - GetActorLocation();
	if (arrows[1])
	{
		AEnemyProjectile* proj = GetWorld()->SpawnActor<AEnemyProjectile>(arrows[1], arrowSummonLocation->GetComponentLocation(), vec.Rotation());
		if (bZombie && proj)
		{
			proj->ChangeProfileName("ZombieProjectile");
		}
	}
	currentState = EArcheressState::IDLE;
}

#pragma endregion
