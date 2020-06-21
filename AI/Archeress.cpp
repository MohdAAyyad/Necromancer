// Fill out your copyright notice in the Description page of Project Settings.


#include "Archeress.h"
#include "ArcheressAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "NecromancerCharacter.h"



AArcheress::AArcheress():AEnemyBase()
{
	hp = 100.0f;
	maxHP = 100.0f;
	baseDamage = 40.0f;
	castDamage = 60.0f;
	damageModifier = 1.0f; //This increases the higher the player's level is
	bDead = false;
	currentState = EArcheressState::IDLE;
	drawTime = 0.5f;
	attackAcceptableDistance = 5000.0f;

	castChance = 0;

	castParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("castParticles"));
	castParticles->SetupAttachment(RootComponent);

	arrowSummonLocation = CreateDefaultSubobject<USceneComponent>(TEXT("arrowSummonLocation"));
	arrowSummonLocation->SetupAttachment(RootComponent);
	GetCapsuleComponent()->SetCollisionProfileName("AliveEnemy");

	castVolume = castPitch = 1.0f;
}
void AArcheress::BeginPlay()
{

	animInstance = Cast<UArcheressAnimInstance>(GetMesh()->GetAnimInstance());

	if (castParticles)
		castParticles->DeactivateSystem();

	Super::BeginPlay();
}
void AArcheress::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (permenantTarget)//Archers don't move. When they see the player, they'll measure the distance and attack if the player is close enough
	{
		Attack();
	}
}

void AArcheress::Attack()
{
	if (GetDistanceToPlayer() <= attackAcceptableDistance && currentState == EArcheressState::IDLE) //If the player is close enough, aim
	{
		if (animInstance)
		{
			if (!animInstance->IsMontagePlaying())
			{
				castChance = FMath::RandRange(0, 40);

				if (castChance >= 0 && castChance <= 10)
				{
					animInstance->NextCast();
					if (castParticles)
						castParticles->ActivateSystem(true);
					if (castSound)
						UGameplayStatics::SpawnSoundAtLocation(GetWorld(), castSound, GetActorLocation(), FRotator::ZeroRotator, castVolume, castPitch, 0.0f, castSound->AttenuationSettings);

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
void AArcheress::TakeRegularDamage(float damage_)
{
	if (!bDead || bZombie)
	{
		hp -= damage_;
		//UE_LOG(LogTemp, Warning, TEXT("HP is %f"), hp);

		if (hp <= 0.5f && !bZombie)
		{
			bTypeOfBPToSpawn = false;
			Death();
		}
		else if (hp <= 0.5f && bZombie)
		{
			EndZombify();
		}
		else
		{
			if (animInstance)
				animInstance->SetHit();
		}
	}

	if (!aiController->GetPlayer() && !bZombie)
	{
		if (aiController)
		{
			if (!bDead || bZombie)
			{
				aiController->SetSeenTarget(GetWorld()->GetFirstPlayerController()->GetPawn());//If the enemy has not seen the player yet, and the player attacks it, it should now see the player
			}
		}
	}
}
void AArcheress::TakeSpellDamage(float damage_, EStatusEffects effect_, float duration_)
{
	//UE_LOG(LogTemp, Warning, TEXT("Taken spell damage %f"), damage_);
	if (!bDead || bZombie)
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

	if (!aiController->GetPlayer() && !bZombie)
	{
		if (aiController)
		{
			if (!bDead || bZombie)
			{
				aiController->SetSeenTarget(GetWorld()->GetFirstPlayerController()->GetPawn());//If the enemy has not seen the player yet, and the player attacks it, it should now see the player
			}
		}
	}
}

void AArcheress::TakeSpellDamage(float damage_)
{
	if (!bDead || bZombie)
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
			if (animInstance)
				animInstance->SetHit();
		}
	}

	if (!aiController->GetPlayer() && !bZombie)
	{
		if (aiController)
		{
			if (!bDead || bZombie)
			{
				aiController->SetSeenTarget(GetWorld()->GetFirstPlayerController()->GetPawn());//If the enemy has not seen the player yet, and the player attacks it, it should now see the player
			}
		}
	}
}


void AArcheress::Death()
{
	Super::Death();
	//Enemy die animation
	if (animInstance)
		animInstance->Death();
	currentState = EArcheressState::DEATH;
	if (castParticles)
		castParticles->DeactivateSystem();
}
#pragma region Zombification

void AArcheress::Zombify()
{
	Super::Zombify();
	if (animInstance)
		animInstance->SetZombify();
	GetWorld()->GetTimerManager().SetTimer(zombifyTimerHandle, this, &AArcheress::EndZombify, zombifyDuration, false);
}


void AArcheress::EndZombify()
{
	Super::EndZombify();
	currentState = EArcheressState::DEATH;
	if (animInstance)
		animInstance->Death();
}

void AArcheress::ActivateZombie()
{
	Super::ActivateZombie();
	currentState = EArcheressState::IDLE;
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
		if (proj)
		{
			if (bZombie)
			{
				proj->ChangeProfileName("ZombieProjectile");
			}
			proj->SetDamage(baseDamage * damageModifier);
			proj->SetParent(this);
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
		if (proj)
		{
			if (bZombie)
			{
				proj->ChangeProfileName("ZombieProjectile");
			}
			proj->SetDamage(castDamage * damageModifier);
			proj->SetParent(this);
		}
	}
	currentState = EArcheressState::IDLE;
}

#pragma endregion
