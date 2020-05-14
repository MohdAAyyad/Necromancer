// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuPawn.h"

// Sets default values
AMainMenuPawn::AMainMenuPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	animInstance = nullptr;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = root;

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	mesh->SetupAttachment(root);


}
void AMainMenuPawn::BeginPlay()
{
	Super::BeginPlay();

	animInstance = Cast<UMMPawnAnimInstance>(mesh->GetAnimInstance());
}

void AMainMenuPawn::Rise(bool value_)
{
	if (animInstance)
		animInstance->SetRise(value_);
}

void AMainMenuPawn::MakeEnemiesRise()
{
	if (animInstance)
		animInstance->SetEnemyRise(true);
}

