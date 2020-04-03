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

void USpellConjuror::ConjurAimSpell(EAimSpells spell_, FVector spawnLocation_, FRotator spawnRotation_, bool increaseHP_, float& currentHP_, float damage_, EStatusEffects status_, EStatusDuration duration_)
{
	int i = static_cast<int>(spell_);
	if ( i <= aimSpellProjectiles.Num() - 1)
	{
		if (aimSpellProjectiles[i])
		{
			AAimProjectile* proj = GetWorld()->SpawnActor<AAimProjectile>(aimSpellProjectiles[i], spawnLocation_, spawnRotation_);

			if (proj)
				proj->SetDamage(damage_); //Adds player base magic damage + spell magic damage to the projectile
		}
	}
}

void USpellConjuror::ConjurBloodSpell(EBloodSpells spell_, FVector spawnLocation_, FRotator spawnRotation_, bool usingASpellSummon_, float& currentHP_, float damage_, EStatusEffects status_, EStatusDuration duration_)
{
	int i = static_cast<int>(spell_);
	if (!usingASpellSummon_) //Is this a summon spell?
	{
		if (i <= bloodSpellProjectiles.Num() - 1)
		{
			if (bloodSpellProjectiles[i])
			{
				AAimProjectile* proj = GetWorld()->SpawnActor<AAimProjectile>(bloodSpellProjectiles[i], spawnLocation_, spawnRotation_);
				if (proj)
					proj->SetDamage(damage_); //Adds player base magic damage + spell magic damage to the projectile
			}
		}
	}
	else
	{
		int index = i - static_cast<int>(EBloodSpells::SUMMONSKELETON);//Summon spells are the last blood spells. SUMMONSKELETON is least summon spell number
		if (index <= summons.Num()) 
		{			
			if (summons[index])
			{
				spawnLocation_.Z -= 200.0f;
				spawnRotation_.Pitch = 0.0f;
				ASummonBase* toBeSummoned = GetWorld()->SpawnActor<ASummonBase>(summons[index], spawnLocation_, spawnRotation_);
				if (toBeSummoned)
				{
					//Summon spell do not affect the spell base damage so we can check it to determine the level of the summon
					if (damage_ >= 40.0f && damage_ < 70.0f)
						toBeSummoned->SetSummonLevel(2);
					else if (damage_ >= 70.0f)
						toBeSummoned->SetSummonLevel(3);
				}
			}
		}
	}
}

void USpellConjuror::ConjurInnateSpell(EInnateSpells spell_, FVector spawnLocation_, EStatusEffects status_, EStatusDuration duration_)
{

}


