// Fill out your copyright notice in the Description page of Project Settings.


#include "BloodTimeBomb.h"
#include "AI/EnemyBase.h"


ABloodTimeBomb::ABloodTimeBomb() : ABloodRocket()
{
	destroyDelay = 2.0f;
	InitialLifeSpan = 100.0f;
	hasBeenAttached = false;
}

void ABloodTimeBomb::OnOverlap(UPrimitiveComponent* overlappedComponent_,
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
			if (!enemy->IsDead() && !enemy->bZombie)
			{
				if (!hasBeenAttached)
				{
					movement->Velocity = FVector::ZeroVector;
					AttachToActor(otherActor_, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
					GetWorld()->GetTimerManager().SetTimer(destroyTimeHandle, this, &ABloodTimeBomb::Explode, destroyDelay, false);
					hasBeenAttached = true;
				}
			}
		}
	}
}

void ABloodTimeBomb::Explode()
{
	if (impact)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impact, GetActorLocation(), FRotator::ZeroRotator, FVector(1.0f, 1.0f, 1.0f));
	if (impactSound)
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), impactSound, GetActorLocation(), FRotator::ZeroRotator, impactVolume, impactPitch, 0.0f, impactSound->AttenuationSettings);

	explosionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetWorld()->GetTimerManager().SetTimer(destroyTimeHandle, this, &ABloodTimeBomb::DestroyProjectile, 0.1f, false);
	if (playerController && cameraShake)
		playerController->PlayerCameraManager->PlayCameraShake(cameraShake, 0.5f);
}