// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUIController.h"


// Sets default values for this component's properties
UPlayerUIController::UPlayerUIController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bGameIsPaused = false;
	hpPercent = 1.0f;
	bloodPercent = 1.0f;
	// ...
}


// Called when the game starts
void UPlayerUIController::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}

// Called every frame
void UPlayerUIController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerUIController::EquipAimSpell(EAimSpells spell_)
{
	SpellsInventory::GetInstance()->EquipNewAimSpell(spell_);
}

void UPlayerUIController::PauseGame()
{
	bGameIsPaused = !bGameIsPaused;
}

float UPlayerUIController::UpdateHealth() const
{
	return hpPercent;
}

float UPlayerUIController::UpdateBlood() const
{
	return bloodPercent;
}

void UPlayerUIController::SetHealthPercentage(float percent_)
{
	hpPercent = percent_;
}

void UPlayerUIController::SetBloodPercentage(float percent_)
{
	bloodPercent = percent_;
}