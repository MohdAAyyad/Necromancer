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

void USpellConjuror::ConjurAimSpell(EAimSpells spell_, FVector spawnLocation_, FRotator spawnRotation_, bool increaseHP_, float& currentHP_, float damage, EStatusEffects status_, EStatusDuration duration_)
{
	int i = static_cast<int>(spell_);
	if ( i <= aimSpellProjectiles.Num() - 1)
	{
		if(aimSpellProjectiles[i])
			GetWorld()->SpawnActor<AAimProjectile>(aimSpellProjectiles[i], spawnLocation_, spawnRotation_);
	}
}

void USpellConjuror::ConjurBloodSpell(EBloodSpells spell_, FVector spawnLocation_, FRotator spawnRotation_, bool increaseHP_, float& currentHP_, float damage, EStatusEffects status_, EStatusDuration duration_)
{
	int i = static_cast<int>(spell_);
	if (i <= bloodSpellProjectiles.Num() - 1)
	{
		if(bloodSpellProjectiles[i])
			GetWorld()->SpawnActor<AAimProjectile>(bloodSpellProjectiles[i], spawnLocation_, spawnRotation_);
	}
}

void USpellConjuror::ConjurInnateSpell(EInnateSpells spell_, FVector spawnLocation_, EStatusEffects status_, EStatusDuration duration_)
{

}


