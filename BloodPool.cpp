#include "BloodPool.h"
#include "TimerManager.h"
#include "BloodWall.h"


ABloodPool::ABloodPool()
{
	PrimaryActorTick.bCanEverTick = true;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = root;

	box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	box->SetCollisionProfileName("BlockAll");
	box->SetupAttachment(root);

	particles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles"));
	particles->SetupAttachment(root);
	bAbsorbed = false;
	bpMaster = nullptr;
}

void ABloodPool::Absorbed()
{
	UE_LOG(LogTemp, Warning, TEXT("Absorbing BP"));
	FTimerHandle handle;
	bAbsorbed = true;
	//UE_LOG(LogTemp, Warning, TEXT("%f"), GetActorScale3D().Size());
	if (GetActorScale3D().Size() > 0.5f)
	{
		SetActorScale3D(FMath::VInterpTo(GetActorScale3D(), FVector::ZeroVector, GetWorld()->GetDeltaSeconds(), 1.0f));
		GetWorld()->GetTimerManager().SetTimer(handle, this, &ABloodPool::Absorbed, GetWorld()->GetDeltaSeconds(), false);
	}
	else
	{
		DestroyBP();
	}
}
void ABloodPool::DestroyBP()
{
	if (bpMaster)
	{
		bpMaster->Destroy();
		Destroy();
	}
}

void ABloodPool::Interact()
{
	if (!bAbsorbed)
	{
		box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Absorbed();
	}
}

void ABloodPool::WallAction()
{
	if (wall)
	{
		FVector vec = GetActorLocation();
		vec.Z -= 120.0f;
		GetWorld()->SpawnActor<ABloodWall>(wall, vec, FRotator::ZeroRotator);
		Absorbed();
	}
}