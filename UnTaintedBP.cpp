// Fill out your copyright notice in the Description page of Project Settings.


#include "UnTaintedBP.h"

AUnTaintedBP::AUnTaintedBP() : ABloodPool()
{
	bloodPoints = healthPoints = 20.0f;
}

void AUnTaintedBP::BeginPlay()
{
	Super::BeginPlay();

	if (particles)
		particles->DeactivateSystem();
}

void AUnTaintedBP::Tick(float deltaTime_)
{
	Super::Tick(deltaTime_);
}

void AUnTaintedBP::Interact(float& value_, bool bp_)
{
	if (!bAbsorbed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Interacted with untainted BP"));
		if (particles)
		{			
			particles->ActivateSystem(true);
		}
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
}