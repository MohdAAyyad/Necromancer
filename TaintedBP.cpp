// Fill out your copyright notice in the Description page of Project Settings.


#include "TaintedBP.h"

ATaintedBP::ATaintedBP() : ABloodPool()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATaintedBP::BeginPlay()
{
	Super::BeginPlay();
}

void ATaintedBP::Tick(float deltaTime_)
{
	Super::Tick(deltaTime_);
}

void ATaintedBP::Interact()
{
	Super::Absorbed();
}