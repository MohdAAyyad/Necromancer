// Fill out your copyright notice in the Description page of Project Settings.


#include "BloodMiasma.h"
#include "AI/EnemyBase.h"

ABloodMiasma::ABloodMiasma():AAimProjectile()
{
	InitialLifeSpan = 100.0f;
	durationInSeconds = 10.0f;
	damage = 0.0f; //Blood Miasma doesn't do any damage
}

void ABloodMiasma::OnOverlap(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ != nullptr && otherComp_ != nullptr && otherActor_ != this)
	{
		//UGameplayStatics::SpawnDecalAttached(decalMaterial, FVector(128.0f, 128.0f, 128.0f), otherActor_->GetRootComponent(),NAME_None,sweepResult_.ImpactPoint,sweepResult_.Normal.Rotation());

		//UGameplayStatics::SpawnDecalAtLocation(GetWorld(), decalMaterial, FVector(128.0f, 128.0f, 128.0f), otherActor_->GetActorLocation(), sweepResult_.Normal.Rotation());

		AEnemyBase* enemy = Cast<AEnemyBase>(otherActor_);

		if (enemy)
		{
			if (enemy->GetCurrentStatusEffect() == EStatusEffects::NONE && !enemy->IsDead()) //An enemy can be affected with only one status effect at a time
			{
				enemy->TakeSpellDamage(damage, EStatusEffects::POISONED, durationInSeconds);
			}
		}
		else
		{
			ADestructibleProp* prop = Cast<ADestructibleProp>(otherActor_);
			if (prop)
			{
				prop->TakeDamage(200.0f);
			}
		}
	}
}
