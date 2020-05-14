// Fill out your copyright notice in the Description page of Project Settings.


#include "GeomancerSpikes.h"

AGeomancerSpikes::AGeomancerSpikes() : AEnemyProjectile()
{
	timeToEnableCollision = 0.3f;
}

void AGeomancerSpikes::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(enableCollisiontimeHandle, this, &AGeomancerSpikes::EnableCollision, timeToEnableCollision, false);

	if (sphere)
		sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGeomancerSpikes::EnableCollision()
{
	if(sphere)
		sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
