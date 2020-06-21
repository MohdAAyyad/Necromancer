// Fill out your copyright notice in the Description page of Project Settings.


#include "MerkvillShield.h"
#include "../AimProjectile.h"

// Sets default values
AMerkvillShield::AMerkvillShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	box->SetGenerateOverlapEvents(true);
	box->SetCollisionProfileName("Pawn"); //Pawn serves the purpose here: allow the camera line cast and prevent the player from physically passing through
	box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	box->SetupAttachment(root);

	shieldParticles0  = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles0"));
	shieldParticles0->SetupAttachment(root);

	shieldParticles1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles1"));
	shieldParticles1->SetupAttachment(root);

	shieldParticles2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles2"));
	shieldParticles2->SetupAttachment(root);

	shieldParticles3 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles3"));
	shieldParticles3->SetupAttachment(root);

	shieldParticles4 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles4"));
	shieldParticles4->SetupAttachment(root);
}

// Called when the game starts or when spawned
void AMerkvillShield::BeginPlay()
{
	Super::BeginPlay();

	if (box)
		box->OnComponentBeginOverlap.AddDynamic(this, &AMerkvillShield::OverlapWithPlayerPorjectiles);

	if (shieldParticles0)
		shieldParticles0->DeactivateSystem();

	if (shieldParticles1)
		shieldParticles1->DeactivateSystem();

	if (shieldParticles2)
		shieldParticles2->DeactivateSystem();

	if (shieldParticles3)
		shieldParticles3->DeactivateSystem();

	if (shieldParticles4)
		shieldParticles4->DeactivateSystem();

	
}

void AMerkvillShield::activateParticles()
{
	UE_LOG(LogTemp, Warning, TEXT("Activate Particles called"));
	if (shieldParticles0)
	{
		shieldParticles0->ActivateSystem(true);
		UE_LOG(LogTemp, Warning, TEXT("Activating particles 0"));
	}
	if (shieldParticles1)
		shieldParticles1->ActivateSystem(true);
	if (shieldParticles2)
		shieldParticles2->ActivateSystem(true);
	if (shieldParticles3)
		shieldParticles3->ActivateSystem(true);
	if (shieldParticles4)
		shieldParticles4->ActivateSystem(true);

	box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
void AMerkvillShield::DestroySelf()
{
	Destroy();
}

void AMerkvillShield::OverlapWithPlayerPorjectiles
(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ != nullptr && otherActor_ != this && otherComp_ != nullptr)
	{
		AAimProjectile* proj = Cast<AAimProjectile>(otherActor_);

		if (proj) //Destroy player projectiles as the player won't be able to damage the boss directly yet
			proj->Destroy();
	}
}
