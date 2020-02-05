// Fill out your copyright notice in the Description page of Project Settings.


#include "UnTaintedBP.h"

AUnTaintedBP::AUnTaintedBP() : ABloodPool()
{
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = root;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetCollisionProfileName("BlockAll");
	mesh->SetGenerateOverlapEvents(true);
	mesh->SetupAttachment(root);

	bloodPoints = healthPoints = 20.0f;
}

void AUnTaintedBP::BeginPlay()
{
	Super::BeginPlay();
}

void AUnTaintedBP::Tick(float deltaTime_)
{
	Super::Tick(deltaTime_);
}

void AUnTaintedBP::Interact(float& value_, bool bp_)
{
	if (bp_) //If true, increase BPs, otherwise, increase health
	{
		value_ += bloodPoints;
	}
	else
	{
		value_ += healthPoints;
	}
	Super::Absorbed();
}