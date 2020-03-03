// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyProjectile.h"
#include "AI/EnemyBase.h"
#include "Necromancer/NecromancerCharacter.h"
#include "BloodWall.h"


AEnemyProjectile::AEnemyProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Die after 2 seconds by default
	InitialLifeSpan = 8.0f;
	movement->InitialSpeed = 500.f;
	movement->MaxSpeed = 500.f;
	damage = 40.0f;
}

void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyProjectile::OnOverlap(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ != nullptr && otherComp_ != nullptr && otherActor_ != this)
	{
		ANecromancerCharacter* player = Cast<ANecromancerCharacter>(otherActor_);
		if (player)
		{
			player->TakeDamage(damage);
			if(impact)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impact, otherActor_->GetActorLocation(),FRotator::ZeroRotator, FVector(0.2f, 0.2f, 0.2f));
			Destroy();
		}
		else
		{
			AEnemyBase* enemy = Cast<AEnemyBase>(otherActor_); //This mainly for zombies
			if (enemy)
			{
				enemy->TakeSpellDamage(damage, EStatusEffects::NONE, 0.0f);
				if (impact)
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impact, otherActor_->GetActorLocation(), FRotator::ZeroRotator, FVector(0.2f, 0.2f, 0.2f));
				Destroy();
			}
			else
			{
				ABloodWall* wall = Cast<ABloodWall>(otherActor_);
				if (wall)
				{
					if (impact)
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impact, otherActor_->GetActorLocation(), FRotator::ZeroRotator, FVector(0.2f, 0.2f, 0.2f));
					wall->TakeDamage(damage);
					Destroy();
				}
			}
		}

	}
}

void AEnemyProjectile::BindSphere()
{
	sphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectile::OnOverlap);
}

void AEnemyProjectile::ChangeProfileName(FString profile_)
{
	UE_LOG(LogTemp, Warning, TEXT("New profile %s"), *profile_);
	sphere->SetCollisionProfileName(FName(*profile_));
}

