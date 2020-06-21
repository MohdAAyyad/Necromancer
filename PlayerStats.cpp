// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStats.h"

// Sets default values for this component's properties
UPlayerStats::UPlayerStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//TODO
	//Get the data from the save file
	//TODO
	maxHP = 350.0f;
	hp = 350.0f;
	maxBP = 250.0f;
	bp = 250.0f;
	baseDamage = 20.0f;
	baseMagicDamage = 20.0f;
	// ...
}


// Called when the game starts
void UPlayerStats::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


float& UPlayerStats::GetMaxHP()
{
	return maxHP;
}
float& UPlayerStats::GetHP()
{
	return hp;
}
float UPlayerStats::GetHPPercent()
{
	if (hp == maxHP)
		return 1.0f;
	return (hp / maxHP);
}
void UPlayerStats::AddToHP(float value_)
{
	hp += value_;
	if (hp >= maxHP)
		hp = maxHP;
}

void UPlayerStats::AddToMaxHP(float amount_) //Called from UI
{
	maxHP += amount_;
	hp = maxHP; //Upgrading the max hp, refills the current hp
} 

float& UPlayerStats::GetMaxBP()
{
	return maxBP;
}
float& UPlayerStats::GetBP()
{
	return bp;
}
float UPlayerStats::GetBPPercent()
{
	if (bp == maxBP)
		return 1.0f;
	return (bp / maxBP);
}

void UPlayerStats::AddToMaxBP(float amount_)//Called from UI
{
	maxBP += amount_;
	bp = maxBP; //Upgrading the max bp, refills the current bp
} 
void UPlayerStats::AddToBP(float value_)
{
	bp += value_;
	if (bp >= maxBP)
		bp = maxBP;
}

float UPlayerStats::GetBaseDamage()
{
	return baseDamage;
}
float UPlayerStats::GetBaseMagicDamage()
{
	return baseMagicDamage;
}

void UPlayerStats::AddToBaseDamage(float amount_)
{
	baseDamage += amount_;
}
void UPlayerStats::AddToBaseMagicDamage(float amount_)
{
	baseMagicDamage += amount_;
}

