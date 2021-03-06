// Fill out your copyright notice in the Description page of Project Settings.


#include "CryptoCharge.h"
#include "AI/EnemyBase.h"
#include "Necromancer/NecromancerCharacter.h"
#include "BloodWall.h"

ACryptoCharge::ACryptoCharge():AEnemyProjectile()
{
	chargeLossDueToWall = 40.0f;
}

void ACryptoCharge::OnOverlap(UPrimitiveComponent* overlappedComponent_,
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
				ABloodWall* wall = Cast<ABloodWall>(otherActor_);
				if (wall)
				{
					wall->PropTakeDamage(damage); //The player can build up walls to decrease the power of the charge ball
					damage -= chargeLossDueToWall;
					if (damage <= 1.0f)
						Destroy();
				}
			}
		}

	}
}