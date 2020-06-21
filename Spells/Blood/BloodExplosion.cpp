// Fill out your copyright notice in the Description page of Project Settings.


#include "BloodExplosion.h"
#include "AI/EnemyBase.h"

ABloodExplosion::ABloodExplosion() : AAimProjectile()
{
	damage = 100.0f;
	InitialLifeSpan = 0.5f;
	explosionRadius = 600.0f;
	sphere->SetSphereRadius(explosionRadius);
}

void ABloodExplosion::OnOverlap(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ != nullptr && otherComp_ != nullptr && otherActor_ != this)
	{
		AEnemyBase* enemy = Cast<AEnemyBase>(otherActor_);

		if (enemy)
		{
			if (enemy->bZombie)//Create a chain reaction between zombies/dead bodies
			{
				enemy->DelayedExplosion(); 
			}
			else if (!enemy->IsDead())
			{
				if (playerController && cameraShake)
					playerController->PlayerCameraManager->PlayCameraShake(cameraShake, 0.5f);
				enemy->TakeSpellDamage(damage);
			}

		}
		else
		{
			ADestructibleProp* prop = Cast<ADestructibleProp>(otherActor_);
			if (prop)
			{
				prop->PropTakeDamage(damage);
			}
		}
	}
}