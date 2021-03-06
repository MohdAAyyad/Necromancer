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

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
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
	playerController = nullptr;
	cameraShake = nullptr;
	bBeingAbsorbed = false;

	impactVolume = 1.0f;
	impactPitch = 1.0f;

	bPlaySpawnSoundOnBeginPlay = false;

	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Spawn Audio Component"));
	audioComponent->SetupAttachment(root);
}

// Called when the game starts or when spawned
void AAimProjectile::BeginPlay()
{
	Super::BeginPlay();
	BindSphere();
	if (audioComponent)
		audioComponent->Sound = spawnSound;

	if (bPlaySpawnSoundOnBeginPlay)
		audioComponent->Play();
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
		AEnemyBase* enemy = Cast<AEnemyBase>(otherActor_);

		if (enemy)
		{
				if (!enemy->IsDead())
				{
					bBeingAbsorbed = enemy->IsEnemyAbsorbingAttack();
					if (!bBeingAbsorbed) //Only if you're not being absorbed, should you damage the enemy
					{
						if (playerController && cameraShake)
							playerController->PlayerCameraManager->PlayCameraShake(cameraShake, 0.5f);
						enemy->TakeRegularDamage(damage);
						Destroy();
					}
				}
		}
		else
		{

			ADestructibleProp* prop = Cast<ADestructibleProp>(otherActor_);
			if (prop)
			{
				prop->PropTakeDamage(damage);
				Destroy();
			}
		}

		if (!bBeingAbsorbed)
		{
			if (impact)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impact, otherActor_->GetActorLocation(), FRotator::ZeroRotator, FVector(2.0f, 2.0f, 2.0f));
			if (impactSound)
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), impactSound, otherActor_->GetActorLocation(), FRotator::ZeroRotator, impactVolume, impactPitch, 0.0f, impactSound->AttenuationSettings);
			if(decalMaterial)
				UGameplayStatics::SpawnDecalAtLocation(GetWorld(), decalMaterial, FVector(128.0f, 128.0f, 128.0f), otherActor_->GetActorLocation(), sweepResult_.Normal.Rotation());
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
void AAimProjectile::SetControllerAndCameraShake(APlayerController* playerController_, TSubclassOf<UPlayerCameraShake> cameraShake_)
{
	playerController = playerController_;
	cameraShake = cameraShake_;
}

void AAimProjectile::GetSmallerOverTime()
{
	//Get Smaller over time
	//Called by Crypto when a projectile is being absorbed
	FVector scale = GetActorScale();
	scale.X -= 0.1f;
	scale.Y -= 0.1f;
	scale.Z -= 0.1f;
	if (scale.X > 0.2f)
	{
		GetWorld()->GetTimerManager().SetTimer(timeHandleToGetReallySmall, this, &AAimProjectile::GetSmallerOverTime, 0.1f, false);
	}
}


void AAimProjectile::UpdateRotation(FRotator rotation_)
{
	movement->bRotationFollowsVelocity = false;
	SetActorRotation(rotation_);
	movement->bRotationFollowsVelocity = true;
}

void AAimProjectile::UpdateVelocity(FVector vel_)
{
	movement->Velocity = vel_;
}