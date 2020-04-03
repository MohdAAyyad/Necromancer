// Fill out your copyright notice in the Description page of Project Settings.


#include "AimProjectile.h"
#include "AI/EnemyBase.h"
#include "AI/EnemyProjectile.h"

// Sets default values
AAimProjectile::AAimProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = root;

	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
	sphere->SetGenerateOverlapEvents(true);
	sphere->SetCollisionProfileName(TEXT("Trigger"));
	sphere->SetupAttachment(root);

	mesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetCollisionProfileName("IgnoreOnlyPawn");
	mesh->SetupAttachment(root);

	movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	movement->UpdatedComponent = root;
	movement->InitialSpeed = 3000.f;
	movement->MaxSpeed = 3000.f;
	movement->bRotationFollowsVelocity = true;
	movement->bShouldBounce = false;


	particles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles"));
	particles->SetupAttachment(root);

	// Die after 2 seconds by default
	InitialLifeSpan = 2.0f;
	damage = 30.0f;

	effect = EStatusEffects::NONE;
	duration = EStatusDuration::MIN;
	durationInSeconds = 0.0f;
}

// Called when the game starts or when spawned
void AAimProjectile::BeginPlay()
{
	Super::BeginPlay();
	BindSphere();
}

// Called every frame
void AAimProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAimProjectile::OnOverlap(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ != nullptr && otherComp_ != nullptr && otherActor_!=this)
	{
		//UGameplayStatics::SpawnDecalAttached(decalMaterial, FVector(128.0f, 128.0f, 128.0f), otherActor_->GetRootComponent(),NAME_None,sweepResult_.ImpactPoint,sweepResult_.Normal.Rotation());

		if (impact)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impact, otherActor_->GetActorLocation(), FRotator::ZeroRotator, FVector(1.0f, 1.0f, 1.0f));

		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), decalMaterial, FVector(128.0f, 128.0f, 128.0f), otherActor_->GetActorLocation(),sweepResult_.Normal.Rotation());
		//UE_LOG(LogTemp, Warning, TEXT("Impact point %f %f %f"), sweepResult_.ImpactPoint.X, sweepResult_.ImpactPoint.Y, sweepResult_.ImpactPoint.Z);

		AEnemyBase* enemy = Cast<AEnemyBase>(otherActor_);

		if (enemy)
		{
			if (!enemy->IsDead())
			{
				enemy->TakeRegularDamage(damage);
				Destroy();
			}
		}
		else
		{

			ADestructibleProp* prop = Cast<ADestructibleProp>(otherActor_);
			if (prop)
			{
				prop->TakeDamage(damage);
				Destroy();
			}
			else //Yes the player can shoot the enemy's projectiles
			{
				AEnemyProjectile* proj = Cast<AEnemyProjectile>(otherActor_);
				if (proj)
					Destroy();
			}
		}
	}
}

void AAimProjectile::BindSphere()
{
	sphere->OnComponentBeginOverlap.AddDynamic(this, &AAimProjectile::OnOverlap);
}

void AAimProjectile::AddDamageModifier(float damageModifier_)
{
	damage += damageModifier_;
}

void AAimProjectile::SetEffectAndDuration(EStatusEffects effect_, EStatusDuration duration_)
{
	effect = effect_;

	switch (duration_) //Min does not increase the duartion in seconds
	{
	case MOD:
		durationInSeconds += 6.0f;
		break;
	case MAX:
		durationInSeconds += 12.0f;
		break;
	default:
		break;
	}
}

void AAimProjectile::SetDamage(float amount_)
{
	damage = amount_;
}

