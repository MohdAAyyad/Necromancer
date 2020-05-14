// Fill out your copyright notice in the Description page of Project Settings.


#include "CryptoAbsorbtionSphere.h"

// Sets default values for this component's properties
UCryptoAbsorbtionSphere::UCryptoAbsorbtionSphere()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	outerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Outer Sphere"));
	outerSphere->SetSphereRadius(300.0f);
	outerSphere->SetCollisionProfileName("OverlapAll");	

	innerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Inner Sphere"));
	innerSphere->SetSphereRadius(150.0f);
	innerSphere->SetCollisionProfileName("OverlapAll");
	// ...
}


// Called when the game starts
void UCryptoAbsorbtionSphere::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (outerSphere)
	{
		outerSphere->OnComponentBeginOverlap.AddDynamic(this, &UCryptoAbsorbtionSphere::OuterSphereOnOverlap);
		outerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (innerSphere)
	{
		innerSphere->OnComponentBeginOverlap.AddDynamic(this, &UCryptoAbsorbtionSphere::InnerSphereOnOverlap);
		innerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


void UCryptoAbsorbtionSphere::SetCryptoRef(ACrypto* ref_)
{
	cryptoRef = ref_;
}

void UCryptoAbsorbtionSphere::OuterSphereOnOverlap(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	UE_LOG(LogTemp, Error, TEXT("Collision detected"));
	if (otherActor_)
	{
		UE_LOG(LogTemp, Error, TEXT("Passed the first if check"));
		//Make the projectiles go towards the inner sphere
		AAimProjectile* proj = Cast<AAimProjectile>(otherActor_);
		if (proj)
		{
			UE_LOG(LogTemp, Error, TEXT("Caught a projectile"));
			FVector directionToInnerSphere = innerSphere->GetComponentLocation() - proj->GetActorLocation();
			proj->UpdateRotation(directionToInnerSphere.Rotation());
			proj->GetSmallerOverTime(); //Make the projectiles shrink over time to show that they're being absorbed
		}
	}
}

void UCryptoAbsorbtionSphere::InnerSphereOnOverlap(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_)
	{
		//Absorb the projectile's damage
		AAimProjectile* proj = Cast<AAimProjectile>(otherActor_);
		if (proj)
		{
			if (cryptoRef)
			{
				cryptoRef->Absorb(proj->GetDamage());
			}
			proj->Destroy();
		}
	}
}

void UCryptoAbsorbtionSphere::ActivateSpheres()
{
	UE_LOG(LogTemp, Warning, TEXT("Activate Spheres"));
	if (outerSphere)
		outerSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if (innerSphere)
		innerSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void UCryptoAbsorbtionSphere::DeactivateSpheres()
{
	UE_LOG(LogTemp, Error, TEXT("Deactivate Spheres"));
	if (outerSphere)
		outerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (innerSphere)
		innerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



