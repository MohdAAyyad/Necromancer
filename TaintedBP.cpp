// Fill out your copyright notice in the Description page of Project Settings.


#include "TaintedBP.h"

ATaintedBP::ATaintedBP() : ABloodPool()
{
}

void ATaintedBP::BeginPlay()
{
	Super::BeginPlay();
}

void ATaintedBP::Tick(float deltaTime_)
{
	Super::Tick(deltaTime_);
}
