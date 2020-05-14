// Fill out your copyright notice in the Description page of Project Settings.


#include "BloodTornado.h"
#include "AI/EnemyBase.h"

ABloodTornado::ABloodTornado():AAimProjectile()
{
	damage = 120.0f;
	box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	box->SetupAttachment(root);
	sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InitialLifeSpan = 15.0f;
}


void ABloodTornado::BeginPlay()
{
	Super::BeginPlay();
	box->OnComponentBeginOverlap.AddDynamic(this, &ABloodTornado::OnOverlap);
}

void ABloodTornado::OnOverlap(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ != nullptr && otherComp_ != nullptr && otherActor_ != this)
	{
		//UGameplayStatics::SpawnDecalAttached(decalMaterial, FVector(128.0f, 128.0f, 128.0f), otherActor_->GetRootComponent(),NAME_None,sweepResult_.ImpactPoint,sweepResult_.Normal.Rotation());

		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), decalMaterial, FVector(128.0f, 128.0f, 128.0f), otherActor_->GetActorLocation(), sweepResult_.Normal.Rotation());
		//UE_LOG(LogTemp, Warning, TEXT("Impact point %f %f %f"), sweepResult_.ImpactPoint.X, sweepResult_.ImpactPoint.Y, sweepResult_.ImpactPoint.Z);

		AEnemyBase* enemy = Cast<AEnemyBase>(otherActor_);

		if (enemy && !enemy->IsDead() && !enemy->bZombie)
		{
			if (playerController && cameraShake)
				playerController->PlayerCameraManager->PlayCameraShake(cameraShake, 0.5f);
			enemy->TakeSpellDamage(damage);
		}
		else
		{
			ADestructibleProp* prop = Cast<ADestructibleProp>(otherActor_);
			if (prop)
			{
				prop->TakeDamage(damage);
			}
		}
	}
}