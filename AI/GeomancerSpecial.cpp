// Fill out your copyright notice in the Description page of Project Settings.


#include "GeomancerSpecial.h"
#include "Necromancer/NecromancerCharacter.h"
#include "BloodWall.h"

AGeomancerSpecial::AGeomancerSpecial() : AEnemyProjectile()
{
	timeToEnableSecondParticle = 0.6f;
	timeToEnableCollision = timeToEnableSecondParticle + 0.7f;

	secondParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Second Particles"));
	secondParticles->SetupAttachment(root);
	InitialLifeSpan = timeToEnableSecondParticle + 0.3f;
}

void AGeomancerSpecial::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(enableSecondParticletimeHandle, this, &AGeomancerSpecial::EnableSecondCollision, timeToEnableSecondParticle, false);
	GetWorld()->GetTimerManager().SetTimer(enableCollisiontimeHandle, this, &AGeomancerSpecial::EnableCollision, timeToEnableCollision, false);

	if (sphere)
		sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (secondParticles)
		secondParticles->DeactivateSystem();
}

void AGeomancerSpecial::EnableCollision()
{
	if (sphere)
		sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AGeomancerSpecial::EnableSecondCollision()
{
	if (particles)
		particles->DeactivateSystem();

	if (secondParticles)
		secondParticles->ActivateSystem(true);
}

void AGeomancerSpecial::OnOverlap(UPrimitiveComponent* overlappedComponent_,
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
			sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			UE_LOG(LogTemp, Warning, TEXT("UMMMMMMMMMM"));
		}
		else
		{
			AEnemyBase* enemy = Cast<AEnemyBase>(otherActor_); //This mainly for zombies
			if (enemy)
			{
				if (!enemy->bZombie && !enemy->IsDead())
				{
					enemy->TakeSpellDamage(damage);
					sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				}
			}
			else
			{
				ABloodWall* wall = Cast<ABloodWall>(otherActor_);
				if (wall)
				{
					wall->TakeDamage(damage);
					sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				}
			}
		}

	}
}


