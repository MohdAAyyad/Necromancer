// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleProp.h"

// Sets default values
ADestructibleProp::ADestructibleProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = root;

	mesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(root);

	box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	box->SetupAttachment(root);

	hp = 1.0f;

	volume = pitch = 1.0f;
}

// Called when the game starts or when spawned
void ADestructibleProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestructibleProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestructibleProp::DelayedDestroy()
{
	Destroy();
}

void ADestructibleProp::PropTakeDamage(float damage_)
{
	if (hp > 0.5f)
	{
		hp -= damage_;
		UE_LOG(LogTemp, Warning, TEXT("hp is: %f"), hp);
		if (hp <= 0.5f)
		{
			if (destructionSound)
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), destructionSound, GetActorLocation(), FRotator::ZeroRotator, volume, pitch, 0.0f, destructionSound->AttenuationSettings);

			mesh->ApplyDamage(2.0f, GetActorLocation(), FVector::ZeroVector, 1.0f);
			mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetWorld()->GetTimerManager().SetTimer(timeToDestroyHandle, this, &ADestructibleProp::DelayedDestroy, 3.0f, false);
		}
	}
}

