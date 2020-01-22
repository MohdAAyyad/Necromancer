// Fill out your copyright notice in the Description page of Project Settings.


#include "AimProjectile.h"

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
	sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	sphere->SetupAttachment(root);

	mesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(root);

	movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	movement->UpdatedComponent = root;
	movement->InitialSpeed = 3000.f;
	movement->MaxSpeed = 3000.f;
	movement->bRotationFollowsVelocity = true;
	movement->bShouldBounce = true;


	particles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles"));
	particles->SetupAttachment(root);

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AAimProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAimProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

