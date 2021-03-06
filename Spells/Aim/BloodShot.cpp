// Fill out your copyright notice in the Description page of Project Settings.


#include "BloodShot.h"
#include "AI/EnemyBase.h"


ABloodShot::ABloodShot(): AAimProjectile()
{
	damage = 150.0f;
	InitialLifeSpan = 0.5f;
}
void ABloodShot::OnOverlap(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ != nullptr && otherComp_ != nullptr && otherActor_ != this)
	{
		if (impact)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impact, otherActor_->GetActorLocation(), FRotator::ZeroRotator, FVector(2.0f, 2.0f, 2.0f));
		if (impactSound)
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), impactSound, otherActor_->GetActorLocation(), FRotator::ZeroRotator, impactVolume, impactPitch, 0.0f, impactSound->AttenuationSettings);

		AEnemyBase* enemy = Cast<AEnemyBase>(otherActor_);

		if (enemy)
		{
			if (!enemy->IsDead() && !enemy->bZombie)
			{
				if (playerController && cameraShake)
					playerController->PlayerCameraManager->PlayCameraShake(cameraShake, 0.5f);
				enemy->TakeSpellDamage(damage);
				sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
