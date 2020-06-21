// Fill out your copyright notice in the Description page of Project Settings.


#include "BloodWall.h"

// Sets default values
ABloodWall::ABloodWall():ADestructibleProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	hp = 100.0f;
	targetLocation = FVector::ZeroVector;
	currentLocation = FVector::ZeroVector;
	SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
}

void ABloodWall::BeginPlay()
{
	Super::BeginPlay();
	currentLocation = targetLocation = GetActorLocation();
	targetLocation.Z += 200.0f;
}

void ABloodWall::Tick(float deltaTime_)
{
	Super::Tick(deltaTime_);

	if (GetActorLocation().Z < targetLocation.Z)
	{
		currentLocation.Z += 5.0f;
		SetActorLocation(currentLocation);
	}
	else
	{
		PrimaryActorTick.bCanEverTick = false;
	}
}

void ABloodWall::PropTakeDamage(float damage_)
{
	if (hp > 0.5f)
	{
		hp -= damage_;
		UE_LOG(LogTemp, Warning, TEXT("hp is: %f"), hp);
		if (hp <= 0.5f)
		{
			WallAction(FRotator::ZeroRotator);
		}
	}
}

void ABloodWall::WallAction(FRotator rotater_) //Called when the player aims at a wall and presses wallaction. Destroys wall
{
	mesh->ApplyDamage(2.0f, GetActorLocation(), FVector::ZeroVector, 1.0f);
	box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorld()->GetTimerManager().SetTimer(timeToDestroyHandle, this, &ABloodWall::DelayedDestroy, 6.0f, false);
}

