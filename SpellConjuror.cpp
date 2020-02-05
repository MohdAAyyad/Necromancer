// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellConjuror.h"

// Sets default values for this component's properties
USpellConjuror::USpellConjuror()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpellConjuror::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USpellConjuror::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USpellConjuror::ConjourAimSpell(EAimSpells spell_, FVector spawnLocation_, bool increaseHP_, float& currentHP_, float damage, EStatusEffects status_, EStatusDuration duration_)
{
	UE_LOG(LogTemp, Warning, TEXT("Aim Spell %d"), spell_);
}

void USpellConjuror::ConjourBloodSpell(EBloodSpells spell_, FVector spawnLocation_, bool increaseHP_, float& currentHP_, float damage, EStatusEffects status_, EStatusDuration duration_)
{

}


