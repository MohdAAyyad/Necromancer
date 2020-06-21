// Fill out your copyright notice in the Description page of Project Settings.


#include "BossInvisibleWall.h"
#include "NecromancerCharacter.h"

// Sets default values
ABossInvisibleWall::ABossInvisibleWall()
{
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	box->SetGenerateOverlapEvents(true);
	box->SetCollisionProfileName("Trigger");
	box->SetupAttachment(root);

	particles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles"));
	particles->SetupAttachment(root);

}

void ABossInvisibleWall::BeginPlay()
{
	Super::BeginPlay();
	
	if (particles)
		particles->DeactivateSystem();

	if (box)
		box->OnComponentEndOverlap.AddDynamic(this, &ABossInvisibleWall::EndOverlap);
}

void ABossInvisibleWall::EndOverlap
(UPrimitiveComponent* overlappedComp_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_)
{
	if (otherActor_ != nullptr && otherActor_!=this && otherComp_ != nullptr)
	{
		ANecromancerCharacter* player = Cast<ANecromancerCharacter>(otherActor_);
		if (player)
		{
			if (particles)
				particles->ActivateSystem(true);
			if (box)
			{
				box->SetGenerateOverlapEvents(false);
				box->SetCollisionProfileName("BlockAll");
			}
		}
	}
}
