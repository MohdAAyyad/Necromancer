// Fill out your copyright notice in the Description page of Project Settings.


#include "Crypto.h"
#include "CryptoAbsorbtionSphere.h"
#include "Components/CapsuleComponent.h"
#include "CryptoAnimInstance.h"

ACrypto::ACrypto():AGeomancer()
{
	currentState = ECryptoState::IDLE;

	totalDamageAbsorbed = 0.0f;
	animInstance = nullptr;
	bHasSummonedPortals = false;
	numberOfPortalsToSummon = 5;
	numberOfPortalsSofar = 0;
	fireRate = 0.2f;
	absorptionDuration = 7.5f;
	maxNumberOfAttacksBeforeReload = 3;
	numberOfAttacksSoFar = 0;
	acceptableAttackDistance = 2000.0f;
	reloadTime = 2.5f;
	absorbChance = 30;
	
	portalLoc0 = CreateDefaultSubobject<USceneComponent>(TEXT("Portal Loc0"));
	portalLoc0->SetupAttachment(RootComponent);
	portalLocations.Push(portalLoc0);
	portalLoc1 = CreateDefaultSubobject<USceneComponent>(TEXT("Portal Loc1"));
	portalLoc1->SetupAttachment(RootComponent);
	portalLocations.Push(portalLoc1);
	portalLoc2 = CreateDefaultSubobject<USceneComponent>(TEXT("Portal Loc2"));
	portalLoc2->SetupAttachment(RootComponent);
	portalLocations.Push(portalLoc2);
	portalLoc3 = CreateDefaultSubobject<USceneComponent>(TEXT("Portal Loc3"));
	portalLoc3->SetupAttachment(RootComponent);
	portalLocations.Push(portalLoc3);
	portalLoc4 = CreateDefaultSubobject<USceneComponent>(TEXT("Portal Loc4"));
	portalLoc4->SetupAttachment(RootComponent);
	portalLocations.Push(portalLoc4);
	portalLoc5 = CreateDefaultSubobject<USceneComponent>(TEXT("Portal Loc5"));
	portalLoc5->SetupAttachment(RootComponent);
	portalLocations.Push(portalLoc5);
	portalLoc6 = CreateDefaultSubobject<USceneComponent>(TEXT("Portal Loc6"));
	portalLoc6->SetupAttachment(RootComponent);
	portalLocations.Push(portalLoc6);
	portalLoc7 = CreateDefaultSubobject<USceneComponent>(TEXT("Portal Loc7"));
	portalLoc7->SetupAttachment(RootComponent);
	portalLocations.Push(portalLoc7);
	portalLoc8 = CreateDefaultSubobject<USceneComponent>(TEXT("Portal Loc8"));
	portalLoc8->SetupAttachment(RootComponent);
	portalLocations.Push(portalLoc8);
	portalLoc9 = CreateDefaultSubobject<USceneComponent>(TEXT("Portal Loc9"));
	portalLoc9->SetupAttachment(RootComponent);
	portalLocations.Push(portalLoc9);

	beamLoc = CreateDefaultSubobject<USceneComponent>(TEXT("Beam Loc"));
	beamLoc->SetupAttachment(RootComponent);


	outerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Outer Sphere"));
	outerSphere->SetSphereRadius(600.0f);
	outerSphere->SetCollisionProfileName("OverlapAll");
	outerSphere->SetupAttachment(RootComponent);

	innerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Inner Sphere"));
	innerSphere->SetSphereRadius(150.0f);
	innerSphere->SetupAttachment(RootComponent);

	absorbIndicator = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Absorb Indicator"));
	absorbIndicator->SetupAttachment(RootComponent);

	absorbSpeed = 300.0f;
	//bHasAlreadyBeenZombified = false;
	zombifyDuration = 60.0f;
}

void ACrypto::BeginPlay()
{
	AEnemyBase::BeginPlay();
	animInstance = Cast<UCryptoAnimInstance>(GetMesh()->GetAnimInstance());

	if (sense)
	{
		sense->OnSeePawn.AddDynamic(this, &ACrypto::OnSeePlayer);
	}

	if (outerSphere)
	{
		outerSphere->OnComponentBeginOverlap.AddDynamic(this, &ACrypto::OuterSphereOnOverlap);
		outerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (innerSphere)
	{
		innerSphere->OnComponentBeginOverlap.AddDynamic(this, &ACrypto::InnerSphereOnOverlap);
		innerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (absorbIndicator)
		absorbIndicator->DeactivateSystem();
}
void ACrypto::Tick(float DeltaTime)
{
	AEnemyBase::Tick(DeltaTime);

	if (permenantTarget && currentState != ECryptoState::DEATH && currentState != ECryptoState::ABSORBING && currentState != ECryptoState::ATTACKING && currentState!= ECryptoState::RELOADING && (GetDistanceToPlayer() <= acceptableAttackDistance))
	{
		Attack();
		currentState = ECryptoState::ATTACKING;
		if (animInstance)
			animInstance->NextCast();
	}
}
void ACrypto::OnSeePlayer(APawn* pawn_)
{
	if (!bDead || bZombie)
	{
		WhoToLookFor(pawn_);

		if (!distractingZombie)
		{
			if (!permenantTarget && !bZombie) //If we're a zombie, permenant target will be filled from inside who to look for
				permenantTarget = pawn_;
			else if (!permenantTarget && bZombie)
				permenantTarget = enemyForZombie;
		}
	}
}

void ACrypto::TakeRegularDamage(float damage_)
{
	if (!bDead || bZombie)
	{
		if(currentState != ECryptoState::ABSORBING)
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
			if (animInstance && currentState!=ECryptoState::ATTACKING && currentState != ECryptoState::ABSORBING) //Don't play hit animation when attacking
				animInstance->SetHit();

			if (hp <= 50.0f && numberOfPortalsToSummon == 5)
			{
				numberOfPortalsToSummon += 5;
				bHasChosenSpell = false;
				bHasSummonedPortals = false;
				fireRate = 0.2f;
			}
		}
	}

	if (!aiController->GetPlayer() && !bZombie)
	{
		OnSeePlayer(GetWorld()->GetFirstPlayerController()->GetPawn()); //If the enemy has not seen the player yet, and the player attacks it, it should now see the player
	}
}
void ACrypto::TakeSpellDamage(float damage_)
{
	if (!bDead || bZombie)
	{
		if (currentState != ECryptoState::ABSORBING)
			hp -= damage_;
		//UE_LOG(LogTemp, Warning, TEXT("HP is %f"), hp);

		if (hp <= 0.5f)
		{
			bTypeOfBPToSpawn = true;
			Death();
		}
		else
		{
			if (animInstance && currentState != ECryptoState::ATTACKING && currentState != ECryptoState::ABSORBING)
				animInstance->SetHit();

			if (hp <= 50.0f && numberOfPortalsToSummon == 5)
			{
				numberOfPortalsToSummon += 5;
				bHasChosenSpell = false;
				bHasSummonedPortals = false;
				fireRate = 0.2f;
			}
		}
	}

	if (!aiController->GetPlayer() && !bZombie)
	{
		OnSeePlayer(GetWorld()->GetFirstPlayerController()->GetPawn()); //If the enemy has not seen the player yet, and the player attacks it, it should now see the player
	}
}
void ACrypto::TakeSpellDamage(float damage_, EStatusEffects effect_, float duration_)
{
	//UE_LOG(LogTemp, Warning, TEXT("Taken spell damage %f"), damage_);
	if (!bDead || bZombie)
	{
		if (currentState != ECryptoState::ABSORBING)
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

			if (animInstance && currentState != ECryptoState::ATTACKING && currentState != ECryptoState::ABSORBING)
				animInstance->SetHit();

			if (hp <= 50.0f && numberOfPortalsToSummon == 5)
			{
				numberOfPortalsToSummon += 5;
				bHasChosenSpell = false;
				bHasSummonedPortals = false;
				fireRate = 0.2f;
			}

		}
	}

	if (!aiController->GetPlayer() && !bZombie)
	{
		OnSeePlayer(GetWorld()->GetFirstPlayerController()->GetPawn()); //If the enemy has not seen the player yet, and the player attacks it, it should now see the player
	}
}
void ACrypto::Attack()
{
	if (permenantTarget)
	{
		if (currentState != ECryptoState::ABSORBING) //Prevent the calls on the stack from firing if you've gone into absorb mode
		{
			if (!bHasChosenSpell && !bHasSummonedPortals)
			{
				if (numberOfPortalsSofar <= numberOfPortalsToSummon - 1)
				{
					if (portalLocations[numberOfPortalsSofar]) //Just being cautious 
					{
						spawnedPortals.Push(GetWorld()->SpawnActor<AEnemyProjectile>
							(portalProjectile, portalLocations[numberOfPortalsSofar]->GetComponentLocation(), portalLocations[numberOfPortalsSofar]->GetComponentRotation()));

						numberOfPortalsSofar++;
					}
				}
				else
				{
					bHasSummonedPortals = true;
					fireRate = 1.0f; //Shooting fire rate
				}
				GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &ACrypto::Attack, fireRate, false); //Portals are summoned, call again to summon projectiles
			}
			else if (!bHasChosenSpell && bHasSummonedPortals)
			{
				FRotator rotation = (permenantTarget->GetActorLocation() - GetActorLocation()).Rotation();
				//Summon the projectiles
				for (int i = 0; i < spawnedPortals.Num(); i++)
				{
					//Spawn the projectiles and add them to the vector
					if (portalLocations[i] && projectiles[i]) //Just being cautious 
					{
						actualProjectiles.Push(GetWorld()->SpawnActor<AEnemyProjectile>
							(projectiles[i], spawnedPortals[i]->GetActorLocation(), rotation));
						actualProjectiles[i]->SetParent(this);
						if (spawnedPortals[i])
							spawnedPortals[i]->SetActorRotation(portalLocations[i]->GetComponentRotation());
					}
				}
				bHasChosenSpell = true;
				GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &ACrypto::Attack, fireRate, false);//Projectiles are spawend, now make them move on the next call
			}
			else
			{
					if (actualProjectiles.Num() > 0)
					{
						for (int i = 0; i < actualProjectiles.Num(); i++)
						{
							FRotator rotation = (permenantTarget->GetActorLocation() - actualProjectiles[i]->GetActorLocation()).Rotation();
							actualProjectiles[i]->SetActorRotation(rotation);
							actualProjectiles[i]->MoveProjectile(1000.0f);
							actualProjectiles[i]->SetHoming(permenantTarget);
							if (bZombie)
								actualProjectiles[i]->ChangeProfileName("ZombieProjectile");
						}
						bHasChosenSpell = false; //Goes to false on the assumption that we haven't finished attacking as we need it to be false for then next Attack() call to spawn new projectiles
						actualProjectiles.Empty(); //Clear the array

						numberOfAttacksSoFar++;
					}
					if (numberOfAttacksSoFar >= maxNumberOfAttacksBeforeReload && (!bDead || bZombie)) //Have we attacked enough? If yes, reload
					{
						if (animInstance)
							animInstance->Reload();

						GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &ACrypto::EndReload, reloadTime, false);
						numberOfAttacksSoFar = 0;
					}
					else if (!bDead || bZombie) //Otherwise, call this function again
					{
						GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &ACrypto::Attack, fireRate, false);
					}
			}
		}
	}
}
void ACrypto::AbsorbMode()
{
	//Go to abosrption state
		if (currentState != ECryptoState::ABSORBING)
		{
			currentState = ECryptoState::ABSORBING;

			//Play animation
			if (animInstance)
			{
				animInstance->NextSpecial();
			}

			//Activate the absorption spheres		
			ActivateSpheres();
			if (absorbIndicator)
				absorbIndicator->ActivateSystem(true);
			//Call this function again to shoot the beam
			GetWorld()->GetTimerManager().SetTimer(absorbHandle, this, &ACrypto::AbsorbMode, absorptionDuration, false);
		}
		else
		{
			//Deactivate the spheres
			DeactivateSpheres();
			//Play the end absorp animation to shoot the beam and the projectiles
			if (animInstance)
				animInstance->ResetSpecial();
			if (absorbIndicator)
				absorbIndicator->DeactivateSystem();
		}
}

void ACrypto::EndAbsorb()
{
	if (permenantTarget)
	{
		FRotator rotation = (permenantTarget->GetActorLocation() - GetActorLocation()).Rotation();
		//Shoot the beam and update its damage
		AEnemyProjectile* proj = GetWorld()->SpawnActor<AEnemyProjectile>(absorbRay, beamLoc->GetComponentLocation(), rotation);
		if (proj)
		{
			proj->SetDamage(totalDamageAbsorbed);
			proj->SetHoming(permenantTarget);
			if(bZombie)
				proj->ChangeProfileName("ZombieProjectile");
			proj->SetParent(this);
			totalDamageAbsorbed = 0.0f; //Reset totalDamageAbsorbed
		}

		//Go into reloading
		currentState = ECryptoState::RELOADING;
		if (animInstance)
			animInstance->Reload();

		GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &ACrypto::EndReload, reloadTime, false);		
	}
}
void ACrypto::Absorb(float damage_) //Called from the absorption sphere
{
	totalDamageAbsorbed += damage_;
	UE_LOG(LogTemp, Error, TEXT("Total Damage has become %f"),totalDamageAbsorbed);
}
bool ACrypto::CalculateAbsorbChance()
{
	if (FMath::RandRange(0, 100) <= absorbChance)
	{
		AbsorbMode();
		return true;
	}
	return false;
}
void ACrypto::ActivateZombie()
{
	if (aiController)
	{
		aiController->ResetDead();
	}
	currentState = ECryptoState::IDLE;
	sense->bOnlySensePlayers = false;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionProfileName("Zombie");
}
void ACrypto::Death()
{
	if (animInstance)
		animInstance->Death();

	bDead = true;
	aiController->SetSeenTarget(nullptr);
	currentState = ECryptoState::DEATH;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (!hasAddedEXP)
		AddEXP();


	enemyForZombie = nullptr;
	permenantTarget = nullptr;
	distractingZombie = nullptr;
}
void ACrypto::Zombify()
{
	bZombie = true;
	if (animInstance)
		animInstance->SetZombify();
	GetWorld()->GetTimerManager().SetTimer(zombifyTimerHandle, this, &ACrypto::EndZombify, zombifyDuration, false);
	hp = 70.0f;
	Super::OnSeePlayer(nullptr);

}
void ACrypto::EndZombify()
{
	currentState = ECryptoState::DEATH;
	if (animInstance)
		animInstance->Death();
}
void ACrypto::EndReload()
{
	if (animInstance)
		animInstance->ResetReload();

	if(!CalculateAbsorbChance()) //Check if crypto is going to absorb mode. If not, just go back to idle and do a regular attack
		currentState = ECryptoState::IDLE;
}

void ACrypto::OuterSphereOnOverlap(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	//UE_LOG(LogTemp, Error, TEXT("Collision detected"));
	if (otherActor_ && otherActor_!= nullptr && otherComp_ != nullptr)
	{
		//Make the projectiles go towards the inner sphere
		AAimProjectile* proj = Cast<AAimProjectile>(otherActor_);
		if (proj)
		{
			//UE_LOG(LogTemp, Error, TEXT("Caught a projectile %s"), *proj->GetName());
			FVector directionToInnerSphere = innerSphere->GetComponentLocation() - proj->GetActorLocation();
			directionToInnerSphere.Normalize();
			proj->UpdateVelocity(directionToInnerSphere*absorbSpeed);
			proj->UpdateRotation(directionToInnerSphere.Rotation());
			proj->GetSmallerOverTime(); //Make the projectiles shrink over time to show that they're being absorbed
		}
	}
}

void ACrypto::InnerSphereOnOverlap(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ && otherActor_ != nullptr && otherComp_ != nullptr)
	{
		//Absorb the projectile's damage
		AAimProjectile* proj = Cast<AAimProjectile>(otherActor_);
		if (proj)
		{
			//UE_LOG(LogTemp, Error, TEXT("Absorbed a projectile %s"), *proj->GetName());
			Absorb(proj->GetDamage());
			proj->Destroy();
		}
	}
}

void ACrypto::ActivateSpheres()
{
	UE_LOG(LogTemp, Warning, TEXT("Activate Spheres"));
	if (outerSphere)
		outerSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if (innerSphere)
		innerSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACrypto::DeactivateSpheres()
{
	UE_LOG(LogTemp, Error, TEXT("Deactivate Spheres"));
	if (outerSphere)
		outerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (innerSphere)
		innerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool ACrypto::IsEnemyAbsorbingAttack()
{
	if (currentState == ECryptoState::ABSORBING)
		return true;
	return false;
}

void ACrypto::SpawnBloodPool() //Overriding it as the Destroy should be called after an attack is done
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
		//When the zombie dies, destroy the body
		bZombie = false;
		AEnemyBase* en = Cast<AEnemyBase>(permenantTarget);
		if (en)
			en->distactingZombieIsDead();
	}
	ClearAndDestroy();
}

void ACrypto::ClearAndDestroy()
{
	//Destroy the portals
	if (spawnedPortals.Num() > 0)
	{
		for (int i = 0; i < spawnedPortals.Num(); i++)
		{
			spawnedPortals[i]->Destroy();//Destroy the portals
		}
		spawnedPortals.Empty();
	}

	//Finish attacking if we have already started
	if (actualProjectiles.Num() > 0)
	{
		for (int i = 0; i < actualProjectiles.Num(); i++)
		{
			FRotator rotation = (permenantTarget->GetActorLocation() - actualProjectiles[i]->GetActorLocation()).Rotation();
			actualProjectiles[i]->SetActorRotation(rotation);
			actualProjectiles[i]->MoveProjectile(1000.0f);
			actualProjectiles[i]->SetHoming(permenantTarget);
			if(bZombie)
				actualProjectiles[i]->ChangeProfileName("ZombieProjectile");
		}
		actualProjectiles.Empty(); //Clear the array
	}

	//Finish the death function
	numberOfPortalsSofar = 0;
	numberOfPortalsToSummon = 5;
	bHasSummonedPortals = false;
	bHasChosenSpell = false;
	permenantTarget = nullptr;
	enemyForZombie = nullptr;

	DeactivateSpheres();
	if (absorbIndicator)
		absorbIndicator->DeactivateSystem();

	if (bZombie) //I have already been zombified once, and I'm dead again, I need to be destroyed
	{
		bZombie = false;
		Destroy();
	}
}