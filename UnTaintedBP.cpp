// Fill out your copyright notice in the Description page of Project Settings.


#include "UnTaintedBP.h"

AUnTaintedBP::AUnTaintedBP() : ABloodPool()
{
	bloodPoints = healthPoints = 20.0f;
}

void AUnTaintedBP::Interact(float& value_, bool bp_)
{
	if (!bAbsorbed)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Interacted with untainted BP"));
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