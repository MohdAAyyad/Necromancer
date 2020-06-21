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

	if (impactSound)
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), impactSound, GetActorLocation(), FRotator::ZeroRotator, impactVolume, impactPitch, 0.0f, impactSound->AttenuationSettings);

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
			player->PlayerTakeDamage(damage);
			sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
						if (!enemy->bZombie && !enemy->IsDead() && enemy != parent && !bBeingAbsorbed)
						{
							enemy->TakeSpellDamageFromZombie(parent, damage);
							Destroy();
						}
					}
					else //Otherwise, you're an enemy attacking a zombie
					{
						if (enemy->bZombie && enemy != parent && !bBeingAbsorbed)
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
						wall->PropTakeDamage(damage);
						Destroy();
					}
					else
					{
						AAimProjectile* proj = Cast<AAimProjectile>(otherActor_);
						if (proj)
						{
							if (bCanPlayerDestroyThisProjectile)
								Destroy();
						}
						else
						{
							ADestructibleProp* prop = Cast<ADestructibleProp>(otherActor_);
							if (prop)
							{
								prop->PropTakeDamage(damage);
								Destroy();
							}
							else
							{
								if (!bBeingAbsorbed)
									Destroy();
							}
						}
					}
				}
			}
		}

	}
}


