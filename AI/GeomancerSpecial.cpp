// Fill out your copyright notice in the Description page of Project Settings.


#include "GeomancerSpecial.h"

AGeomancerSpecial::AGeomancerSpecial() : AEnemyProjectile()
{
	timeToEnableSecondParticle = 0.6f;
	timeToEnableCollision = timeToEnableSecondParticle + 0.7f;

	secondParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Second Particles"));
	secondParticles->SetupAttachment(root);
	InitialLifeSpan = timeToEnableSecondParticle + 0.3f;
}

void AGeomancerSpecial::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(enableSecondParticletimeHandle, this, &AGeomancerSpecial::EnableSecondCollision, timeToEnableSecondParticle, false);
	GetWorld()->GetTimerManager().SetTimer(enableCollisiontimeHandle, this, &AGeomancerSpecial::EnableCollision, timeToEnableCollision, false);

	if (sphere)
		sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (secondParticles)
		secondParticles->DeactivateSystem();
}

void AGeomancerSpecial::EnableCollision()
{
	if (sphere)
		sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AGeomancerSpecial::EnableSecondCollision()
{
	if (particles)
		particles->DeactivateSystem();

	if (secondParticles)
		secondParticles->ActivateSystem(true);
}

