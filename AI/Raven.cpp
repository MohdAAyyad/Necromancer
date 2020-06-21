// Fill out your copyright notice in the Description page of Project Settings.


#include "Raven.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"

ARaven::ARaven():AEnemyBase()
{
	bDead = true; //Starts dead	
	zombifyDuration = 60.0f;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	sense->bOnlySensePlayers = false;
	bAttacking = false;
	fireRate = 1.5f;
	aimProjSpawnLoc = CreateDefaultSubobject<USceneComponent>(TEXT("AimProjSpawnLoc"));
	aimProjSpawnLoc->SetupAttachment(RootComponent);
}

void ARaven::BeginPlay()
{
	Super::BeginPlay();

	animInstance = Cast<URavenAnimInstance>(GetMesh()->GetAnimInstance());
	if (sense)
	{
		sense->OnSeePawn.AddDynamic(this, &ARaven::OnSeePlayer);
	}
}
void ARaven::Tick(float deltaTime_)
{
	Super::Tick(deltaTime_);
}
void ARaven::OnSeePlayer(APawn* pawn_)
{
	if (!bDead || bZombie)
	{
		WhoToLookFor(pawn_);

		if (!permenantTarget && bZombie)
			permenantTarget = enemyForZombie;
	}
}
void ARaven::EndZombify()
{
	if (animInstance)
		animInstance->Death();
}
void ARaven::Attack()
{
	if (animInstance)
		animInstance->NextAttack();
	bAttacking = true;
	GetWorld()->GetTimerManager().SetTimer(fireRateHandle, this, &ARaven::Attack, fireRate, false);

}
void ARaven::TakeRegularDamage(float damage_)
{
	if (bZombie)
	{
		hp -= damage_;
		if (hp <= 0.5f)
		{
			EndZombify();
		}
		else if (!bAttacking)
		{
			if (animInstance)
				animInstance->SetHit();
		}
	}

}
void ARaven::SpawnAimProj()
{
	//Shoot proj at target. Won't work unless you're a zombie as the ravens will start dead (no need to check for bZombie)
	if (aimProj)
	{
		AEnemyProjectile* proj = GetWorld()->SpawnActor<AEnemyProjectile>(aimProj, aimProjSpawnLoc->GetComponentLocation(), GetActorRotation());

		if (proj)
		{
			proj->ChangeProfileName("ZombieProjectile");
			proj->SetParent(this);
			bAttacking = false;
		}
	}
}
void ARaven::SpawnSkeleton()
{
	if (skeletonSpawnBP)
	{
		//Rotate towards where the Raven is looking 
		//FRotator rot = (skeletonSpawnBP->GetActorLocation() - (GetArrowComponent()->GetForwardVector()*300.0f)).Rotation();
		FRotator rot = FRotator::ZeroRotator;
		ASummonBase* toBeSummoned = GetWorld()->SpawnActor<ASummonBase>(summon, skeletonSpawnBP->GetActorLocation(), rot);
		skeletonSpawnBP->Interact();
		Attack();
	}
}

void ARaven::PlaySpawnParticles()
{
	if(skeletonSpawnBP)
		if (magicCircle) //Spawn magic circle under corpse
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), magicCircle, skeletonSpawnBP->GetActorLocation(), FRotator::ZeroRotator, FVector(1.0f, 1.0f, 1.0f));
}
void ARaven::Zombify()
{
	bZombie = true;
	if (animInstance)
		animInstance->SetZombify();
	GetWorld()->GetTimerManager().SetTimer(zombifyTimerHandle, this, &ARaven::EndZombify, zombifyDuration, false);
	hp = 70.0f;
}
void ARaven::ActivateZombie()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionProfileName("Zombie");

	if (skeletonSpawnBP)//If we have a BP reference, summon a skeleton
	{
		if (animInstance)
			animInstance->SummonSkeleton();
	}
	else //Otherwise attack
	{
		Attack();
	}	
}

void ARaven::DestroySelf()
{
	Destroy();
}