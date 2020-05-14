// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyProjectile.h"
#include "AI/EnemyBase.h"
#include "Necromancer/NecromancerCharacter.h"
#include "Spells/Blood/SummonBase.h"
#include "BloodWall.h"


AEnemyProjectile::AEnemyProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Die after 2 seconds by default
	InitialLifeSpan = 8.0f;
	movement->InitialSpeed = 500.f;
	movement->MaxSpeed = 500.f;
	damage = 40.0f;
	parent = nullptr;
}

void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyProjectile::OnOverlap(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ != nullptr && otherComp_ != nullptr && otherActor_ != this)
	{
		if (impact)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impact, otherActor_->GetActorLocation(), FRotator::ZeroRotator, FVector(0.2f, 0.2f, 0.2f));

		ANecromancerCharacter* player = Cast<ANecromancerCharacter>(otherActor_);
		if (player)
		{
			player->TakeDamage(damage);
			Destroy();
		}
		else
		{
			AEnemyBase* enemy = Cast<AEnemyBase>(otherActor_); //This mainly for zombies
			if (enemy)
			{
				if (parent)
				{
					if (parent->bZombie) //If the parent is a zombie, then make sure it's attacking non-zombie enemies and updating the distracting enemy reference
					{
						if (!enemy->bZombie && !enemy->IsDead() && enemy != parent)
						{
							enemy->TakeSpellDamageFromZombie(parent, damage);
							Destroy();
						}
					}
					else //Otherwise, you're an enemy attacking a zombie
					{
						if (enemy->bZombie && enemy != parent)
						{
							enemy->TakeRegularDamage(damage);
							Destroy();
						}
					}
				}
			}
			else
			{
				ASummonBase* summon = Cast<ASummonBase>(otherActor_);
				if (summon)
				{
					summon->SummonTakeDamage(damage);
					Destroy();
				}
				else
				{
					ABloodWall* wall = Cast<ABloodWall>(otherActor_);
					if (wall)
					{
						wall->TakeDamage(damage);
						Destroy();
					}
					else
					{
						//AAimProjectile* proj = Cast<AAimProjectile>(otherActor_);
						//if (proj)
						Destroy();
					}
				}
			}
		}

	}
}

void AEnemyProjectile::BindSphere()
{
	sphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectile::OnOverlap);
}

void AEnemyProjectile::ChangeProfileName(FString profile_)
{
	//UE_LOG(LogTemp, Warning, TEXT("New profile %s"), *profile_);
	sphere->SetCollisionProfileName(FName(*profile_));
}

void AEnemyProjectile::MoveProjectile(float speed_)
{
	//Projectiles that start with zero velocity should not get destroyed until they start moving
	movement->InitialSpeed = movement->MaxSpeed = speed_;
	movement->Velocity = GetActorForwardVector()*speed_;
	FTimerHandle timeToDestroy;
	GetWorld()->GetTimerManager().SetTimer(timeToDestroy, this, &AEnemyProjectile::DestroyProjectile, 6.0f, false);
}

void AEnemyProjectile::DestroyProjectile()
{
	Destroy();
}

void AEnemyProjectile::SetParent(class AEnemyBase* parent_)
{
	//UE_LOG(LogTemp, Warning, TEXT("Set parent has been called"));
	parent = parent_;
}

void AEnemyProjectile::SetHoming(APawn* target)
{
	movement->HomingTargetComponent = target->GetRootComponent();
	movement->bIsHomingProjectile = true;
	movement->HomingAccelerationMagnitude = movement->Velocity.Size();
}
