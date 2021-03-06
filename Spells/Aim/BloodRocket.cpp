// Fill out your copyright notice in the Description page of Project Settings.


#include "BloodRocket.h"
#include "AI/EnemyBase.h"

ABloodRocket::ABloodRocket(): AAimProjectile()
{
	explosionRadius = 80.0f;
	explosionDamage = 50.0f;
	damage = 10.0f;
	InitialLifeSpan = 5.0f;
	destroyDelay = 0.015f;

	explosionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Explosion Sphere"));
	explosionSphere->SetCollisionProfileName("OverlapAllDynamic");
	explosionSphere->SetSphereRadius(explosionRadius);
	explosionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	explosionSphere->SetupAttachment(root);

}

void ABloodRocket::BeginPlay()
{
	Super::BeginPlay();
	if (explosionSphere)
	{
		explosionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABloodRocket::OnExplosionOverlap);
		explosionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABloodRocket::OnOverlap(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ != nullptr && otherComp_ != nullptr && otherActor_ != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("Rocket damaged enemy"));
		//Doesn't matter what you hit, explode anyway
		if (impact)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impact, otherActor_->GetActorLocation(), FRotator::ZeroRotator, FVector(5.0f, 5.0f, 5.0f));

		if (impactSound)
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), impactSound, otherActor_->GetActorLocation(), FRotator::ZeroRotator, impactVolume, impactPitch, 0.0f, impactSound->AttenuationSettings);

		explosionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetWorld()->GetTimerManager().SetTimer(destroyTimeHandle, this, &ABloodRocket::DestroyProjectile, destroyDelay, false);

		//If we hit an enemy, damage the enemy with the primary damage
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
				sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
}

void ABloodRocket::OnExplosionOverlap(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ != nullptr && otherComp_ != nullptr && otherActor_ != this)
	{
		AEnemyBase* enemy = Cast<AEnemyBase>(otherActor_);

		if (enemy && !enemy->IsDead() && !enemy->bZombie)
		{
			enemy->TakeSpellDamage(explosionDamage);
		}
		else
		{
			ADestructibleProp* prop = Cast<ADestructibleProp>(otherActor_);
			if (prop)
			{
				prop->PropTakeDamage(damage);
				sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
}

void ABloodRocket::DestroyProjectile()
{
	Destroy();
}

