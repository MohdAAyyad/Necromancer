// Fill out your copyright notice in the Description page of Project Settings.


#include "Merkvill.h"
#include "Necromancer/NecromancerCharacter.h"
#include "Spells/Blood/SummonBase.h"
#include "BloodWall.h"
#include "../QuestManager.h"

AMerkvill::AMerkvill()
{
	maxArmor = armor = 100.0f;
	maxHP = hp = 750.0f;
	currentState = EMerkvillState::TALKING;
	currentPhase = EMerkvillPhase::PHASE0;
	fireRate = 0.1f;
	maxNumberOfFireBalls = 6;
	numberOfFireBalls = 0;
	reloadTime = 2.0f;
	maxNumberOfMeteors = 6;
	maxNumberOfTornados = 6;
	attackCount = 0;

	explosionSphere = CreateDefaultSubobject< USphereComponent>(TEXT("Explosion Sphere"));
	explosionSphere->SetupAttachment(RootComponent);
	explosionSphere->SetCollisionProfileName("AliveEnemyProjectile");
	explosionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	explosionIndicatorParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Explosion Inidcator Particles"));
	explosionIndicatorParticles->SetupAttachment(RootComponent);

	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	audioComponent->SetupAttachment(RootComponent);

	explosionSphere->SetSphereRadius(64.0f);
	explosionDamage = 100.0f;; //Increased near low health.
	timeToExplode = 6.0f; //Increased near low health to allow the player to kill Merkvill.
	timeUntilNextExplosion = 25.0f; //How long to wait before you're allowed to explode again after the initial explosion
	bCanExplode = true;

	currentLocationIndex = 0;
	startingLocation = FVector::ZeroVector;

	questName = "No longer safe";
	questDescriptionStartFight = "Leah tracked those who stole the scroll from the Raven scouts.\nTo her surprise she found an old acquaintance, Merkvill.\nIt seems he killed the Steel Army soldiers and took the scroll.\nLeah must defeat this shadow from the past \nto complete her mission.";
	questDescriptionEnd = "After a fierce battle, Leah managed to defeat Merkvill.\nAll she had to do now is get the scroll\nand head back to Blood Haven.";

	bFightHasStarted = false;
}

void AMerkvill::BeginPlay()
{
	Super::BeginPlay();
	animInstance = Cast<UMerkvillAnimInstance>(GetMesh()->GetAnimInstance());

	if(sense)
		sense->OnSeePawn.AddDynamic(this, &AMerkvill::OnSeePlayer);

	if (explosionSphere)
		explosionSphere->OnComponentBeginOverlap.AddDynamic(this, &AMerkvill::ExplosionOverlap);

	if (explosionIndicatorParticles)
		explosionIndicatorParticles->DeactivateSystem();

	if (audioComponent)
		if (explosionCharge)
			audioComponent->Sound = explosionCharge;
}
 void AMerkvill::Tick(float DeltaTime)
 {
	 Super::Tick(DeltaTime);
 }

 void AMerkvill::OnSeePlayer(APawn* pawn_)
 {
	 if (!bDead)
	 {
		 WhoToLookFor(pawn_);
		 permenantTarget = pawn_;
	 }
 }
#pragma region DamageCalc
void AMerkvill::Death() //Called from animation
{
	//Spawn auto dialogue at the player
	if(endDialogueIndicator)
		GetWorld()->SpawnActor<AAutoDialogue>(endDialogueIndicator, permenantTarget->GetActorLocation(), permenantTarget->GetActorRotation());
	//Spawn auto dialogue around the Merkvill
	if (endDialogueFinal)
	{
		AMerkvillEndDialogue* finalDialogue = GetWorld()->SpawnActor<AMerkvillEndDialogue>(endDialogueFinal, GetActorLocation(), FRotator::ZeroRotator);
		if (finalDialogue)
			finalDialogue->SetMerkvillRef(this);	
	}

	if (aiController)
		aiController->SetSeenTarget(nullptr);

	QuestManager::GetInstance()->AddQuest(questName, questDescriptionEnd);
}
void AMerkvill::TakeRegularDamage(float damage_)
{
	if (!bDead)
	{
		if (armor > 0.5f)
		{
			armor -= damage_; //Take care of the armor first
			if (armor <= 0.5f)
			{
				if (shieldRef)
					shieldRef->DestroySelf();

				currentPhase = EMerkvillPhase::PHASE1;
			}
		}
		else
		{
			hp -= damage_;
			if (hp <= 0.5f)
			{
				bDead = true;
				if (animInstance)
					animInstance->Death();
				if (explosionSphere)
					explosionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				if (explosionIndicatorParticles)
					explosionIndicatorParticles->DeactivateSystem();
			}
			else //If you don't have armor (not phase0) and you're not dead, you might evade
			{
				if (currentState != EMerkvillState::ATTACKING) //If you're attacking, don't evade
				{
					int evadeChance = FMath::RandRange(0, 101);
					if (evadeChance >= 0 && evadeChance < 50)
					{
						currentState = EMerkvillState::EVADING;
						Evade();
					}
				}
			}
		}
	}
}
void AMerkvill::TakeSpellDamage(float damage_)
{
	TakeRegularDamage(damage_);
}
void AMerkvill::TakeSpellDamage(float damage_, EStatusEffects effect_, float duration_)
{
	TakeRegularDamage(damage_);
}

void AMerkvill::TakeSpellDamageFromZombie(APawn* zombie_, float damage_)
{
	TakeRegularDamage(damage_);
}
#pragma endregion
#pragma region Shield
void AMerkvill::StartFight()
{
	currentState = EMerkvillState::IDLE; //Idle starts the fight
	if (shieldRef)
	{
		shieldRef->activateParticles();
	}
	currentPhase = EMerkvillPhase::PHASE0;
	armor = maxArmor;
	hp = maxHP;
	startingLocation = GetActorLocation();
	QuestManager::GetInstance()->AddQuest(questName, questDescriptionStartFight);
	bFightHasStarted = true;
	Attack();
}

void AMerkvill::SetShieldRef(AMerkvillShield* ref_)
{
	shieldRef = ref_;
}
#pragma endregion
#pragma region Attacking
void AMerkvill::Attack()
{
	if (!bDead)
	{
		int chance = FMath::RandRange(0, 11);
		if (currentState == EMerkvillState::IDLE)
		{
			switch (currentPhase)
			{
			case EMerkvillPhase::PHASE0: //Fireballs only
				if (animInstance)
					animInstance->NextAttack();
				currentState = EMerkvillState::ATTACKING;
				attackCount = 1;
				break;
			case EMerkvillPhase::PHASE1:
				currentState = EMerkvillState::ATTACKING;
				if (chance >= 0 && chance <= 5) //Fireballs and Meteors
				{
					if (animInstance)
					{
						//Attacks are chained.
						//The animation state machine will make sure the animation transition occurs correctly
						animInstance->NextAttack();
						animInstance->NextAttack1();
					}

				}
				else
				{
					if (animInstance) //Fireballs and tornadoes
					{
						animInstance->NextAttack();
						animInstance->NextAttack2();
					}

				}
				attackCount = 2;
				break;
			case EMerkvillPhase::PHASE2:

				InitiateExplosion();
				currentState = EMerkvillState::ATTACKING;
				attackCount = 1;
				break;
			}
		}
	}
}
#pragma endregion
#pragma region Spells
void AMerkvill::FireBall()
{
	if (!bDead)
	{
		if (spells[0])
		{
			FRotator rot = (permenantTarget->GetActorLocation() - GetActorLocation()).Rotation();
			if (numberOfFireBalls < maxNumberOfFireBalls)
			{
				FVector spawnLoc = GetActorLocation();
				spawnLoc.X += FMath::RandRange(-500.0f, 500.0f);
				spawnLoc.Y += FMath::RandRange(-500.0f, 500.0f);

				AEnemyProjectile* proj = GetWorld()->SpawnActor<AEnemyProjectile>(spells[0], spawnLoc, rot);
				if (proj != nullptr)
				{
					proj->SetHoming(permenantTarget);
					proj->SetParent(this);
				}


				GetWorld()->GetTimerManager().SetTimer(fireRateHandle, this, &AMerkvill::FireBall, fireRate, false);
				numberOfFireBalls++;
			}
			else
			{
				numberOfFireBalls = 0;
			}
		}
	}
}
void AMerkvill::Meteors()
{
	if (!bDead)
	{
		if (spells[1])
		{
			float direction = 0.0f;
			if (permenantTarget->GetActorLocation().X > GetActorLocation().X || permenantTarget->GetActorLocation().Y > GetActorLocation().Y)
			{
				direction = 180.0f; //If the player is behind us, rotate the meteors correctly
			}
			FRotator rot = FRotator(-16.0f, -144.0f + direction, 60.0f); //The numbers were obtained through testing in the scene
			for (int i = 0; i < maxNumberOfMeteors; i++)
			{
				FVector spawnLoc = GetActorLocation();
				spawnLoc.X += FMath::RandRange(-2000.0f, 2000.0f);
				spawnLoc.Y += FMath::RandRange(-1000.0f, -1000.0f);
				spawnLoc.Z += 700.0f;


				AEnemyProjectile* proj = GetWorld()->SpawnActor<AEnemyProjectile>(spells[1], spawnLoc, rot);
				if (proj != nullptr)
				{
					proj->SetHoming(permenantTarget);
					proj->SetParent(this);
				}
			}
		}
	}
}
void AMerkvill::Tornado()
{
	if (!bDead)
	{
		if (spells[2])
		{
			FRotator rot = (permenantTarget->GetActorLocation() - GetActorLocation()).Rotation();
			float spawnAllignment = 1.0f; //Left or Right

			FVector spawnLoc = GetActorLocation();

			//Spawn the first one at Merkvill's location
			AEnemyProjectile* proj = GetWorld()->SpawnActor<AEnemyProjectile>(spells[2], spawnLoc, rot);
			if (proj != nullptr)
			{
				proj->SetHoming(permenantTarget);
				proj->SetParent(this);
			}

			spawnLoc.X += 500.0f * spawnAllignment;
			spawnLoc.Y -= 500.0f * spawnAllignment;

			for (int i = 0, j = 0; i < maxNumberOfTornados; i++)
			{
				//Spawn the tornadoes next to each other
				spawnLoc.X += 150.0f*j*spawnAllignment;
				spawnLoc.Y -= 150.0f*j*spawnAllignment;
				j++;
				if (i == 0.5f* maxNumberOfTornados) //Half of the tornadoes on the right, and the other half on the left
				{
					spawnAllignment = -1.0f;
					spawnLoc = GetActorLocation();
					spawnLoc.X += 500.0f * spawnAllignment;
					spawnLoc.Y -= 500.0f * spawnAllignment;
					j = 0;
				}

				AEnemyProjectile* proj_ = GetWorld()->SpawnActor<AEnemyProjectile>(spells[2], spawnLoc, rot);
				if (proj_ != nullptr)
				{
					proj_->SetHoming(permenantTarget);
					proj_->SetParent(this);
				}
			}
		}
	}
}
#pragma endregion
#pragma region Reloading
void AMerkvill::EndReload()
{
	//End reload acts like the hub that determines what the next action is

	currentState = EMerkvillState::IDLE;

	if (currentPhase == EMerkvillPhase::PHASE1 && hp <= 0.5f*maxHP && bCanExplode)
		{
			currentPhase = EMerkvillPhase::PHASE2;
		}
	else if (currentPhase == EMerkvillPhase::PHASE2) //If you're coming back from an explosion, go back to phase 1
		{
			currentPhase = EMerkvillPhase::PHASE1;
		}

	Attack();
}

void AMerkvill::Reload()
{
	attackCount--;
	if (attackCount <= 0) //Make sure not to go into reloading until all attacks have been finished
	{
		currentState = EMerkvillState::RELOADING;
		GetWorld()->GetTimerManager().SetTimer(reloadHandle, this, &AMerkvill::EndReload, reloadTime, false);
	}
}
#pragma endregion
#pragma region Explosion
void AMerkvill::InitiateExplosion()
{
	if (!bDead)
	{
		if (audioComponent)
			audioComponent->Play();
		if (animInstance) //Go into explosion animation
			animInstance->NextAttack3();

		//Enable particle system
		if (explosionIndicatorParticles)
			explosionIndicatorParticles->ActivateSystem(true);

		//After a little while, complete the explosion
		GetWorld()->GetTimerManager().SetTimer(timeToExplodeHandle, this, &AMerkvill::Explode, timeToExplode, false);
	}
}

void AMerkvill::Explode()
{
	if (!bDead)
	{
		if (audioComponent)
			audioComponent->Stop();
		if (animInstance)
			animInstance->CompleteExplosion();

		if (explosionSphere)
			explosionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		if (explosionIndicatorParticles)
			explosionIndicatorParticles->DeactivateSystem();
	}

}

void AMerkvill::ExplosionOverlap
(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (!bDead)
	{
		if (otherActor_ != nullptr && otherComp_ != nullptr && otherActor_ != this)
		{
			if (explosionImpact)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionImpact, otherActor_->GetActorLocation(), FRotator::ZeroRotator, FVector(1.0f, 1.0f, 1.0f));

			ANecromancerCharacter* player_ = Cast<ANecromancerCharacter>(otherActor_);
			if (player_)
			{
				player_->PlayerTakeDamage(explosionDamage);
			}
			else
			{
				AEnemyBase* enemy = Cast<AEnemyBase>(otherActor_); //This mainly for zombies
				if (enemy)
				{
					if (enemy->bZombie)
					{
						enemy->TakeRegularDamage(explosionDamage);
					}
				}
				else
				{
					ASummonBase* summon = Cast<ASummonBase>(otherActor_);
					if (summon)
					{
						summon->SummonTakeDamage(explosionDamage);
					}
					else
					{
						ABloodWall* wall = Cast<ABloodWall>(otherActor_);
						if (wall)
						{
							wall->PropTakeDamage(explosionDamage);
						}
					}
				}
			}

		}
	}
}


void AMerkvill::CanExplodeAgain()
{
	bCanExplode = true;
}

void AMerkvill::ResetExplosionSphere()
{
	//Called from the animation
	if(explosionSphere)
		explosionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorld()->GetTimerManager().SetTimer(timeToExplodeHandle, this, &AMerkvill::CanExplodeAgain, timeUntilNextExplosion, false);
}
#pragma endregion

#pragma region Evasion
void AMerkvill::Evade()
{
	FVector teleportLoc = startingLocation;

	//Emit particles at your location
	if (evadeParticles)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), evadeParticles, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 150.0f), FRotator::ZeroRotator, FVector(1.0f, 1.0f, 1.0f));

	if (currentLocationIndex == 0) //At starting location, go forward
	{
		teleportLoc.X += 1000.0f;
		teleportLoc.Y += 1000.0f;
		currentLocationIndex = 1;
	}
	else if (currentLocationIndex == 1) //Forwards, go backwards
	{
		teleportLoc.X -= 1000.0f;
		teleportLoc.Y -= 1000.0f;
		currentLocationIndex = 2;
	}
	else //Back to starting location
	{
		currentLocationIndex = 0;
	}

	//Emit particles at destination
	SetActorLocation(teleportLoc);
	teleportLoc.Z -= 150.0f;

	if (evadeParticles)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), evadeParticles, teleportLoc, FRotator::ZeroRotator, FVector(1.0f, 1.0f, 1.0f));
	if(evadeSound)
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), evadeSound, GetActorLocation(), FRotator::ZeroRotator, 1.0f, 1.0f, 0.0f, evadeSound->AttenuationSettings);

}
#pragma endregion