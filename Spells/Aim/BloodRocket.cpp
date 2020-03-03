// Fill out your copyright notice in the Description page of Project Settings.


#include "BloodRocket.h"
#include "AI/EnemyBase.h"

ABloodRocket::ABloodRocket(): AAimProjectile()
{
	explosionRadius = 80.0f;
	explosionDamage = 50.0f;
	damage = 10.0f;
	InitialLifeSpan = 5.0f;
	destroyDelay = 0.2f;

	explosionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Explosion Sphere"));
	explosionSphere->SetCollisionProfileName("OverlapAllDynamic");
	explosionSphere->SetSphereRadius(explosionRadius);
	explosionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	explosionSphere->SetupAttachment(root);

}

void ABloodRocket::BeginPlay()
{
	Super::BeginPlay();
	explosionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABloodRocket::OnExplosionOverlap);
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
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impact, otherActor_->GetActorLocation(), FRotator::ZeroRotator, FVector(1.0f, 1.0f, 1.0f));
		explosionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetWorld()->GetTimerManager().SetTimer(destroyTimeHandle, this, &ABloodRocket::DestroyProjectile, destroyDelay, false);

		//If we hit an enemy, damage the enemy with the primary damage
		AEnemyBase* enemy = Cast<AEnemyBase>(otherActor_);

		if (enemy)
		{
			if (!enemy->IsDead())
			{
				enemy->TakeSpellDamage(damage, EStatusEffects::NONE, 0.0f);
				sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
		else
		{
			ADestructibleProp* prop = Cast<ADestructibleProp>(otherActor_);
			if (prop)
			{
				prop->TakeDamage(damage);
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

		if (enemy)
		{
			enemy->TakeSpellDamage(explosionDamage, EStatusEffects::NONE, 0.0f);
		}
		else
		{
			ADestructibleProp* prop = Cast<ADestructibleProp>(otherActor_);
			if (prop)
			{
				prop->TakeDamage(damage);
				sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
}

void ABloodRocket::DestroyProjectile()
{
	Destroy();
}

