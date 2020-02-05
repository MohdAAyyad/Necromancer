// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellsInventory.h"

TUniquePtr<SpellsInventory, TDefaultDelete<SpellsInventory>> SpellsInventory::instance = nullptr;
#pragma region Constructor/Destructon



SpellsInventory::SpellsInventory()
{
	aimSpellsCount = 2;
	bloodSpellsCount = 2;
	maxAimSpellsCount = 4;
	maxBloodSpellsCount = 4;
	currentInnateSpell = EInnateSpells::INNATENONE;

	EquipNewAimSpell(EAimSpells::BLOODRAIN);
	EquipNewAimSpell(EAimSpells::BLOODSPEAR);
	EquipNewAimSpell(EAimSpells::BLOODSPRAY);
}

SpellsInventory::~SpellsInventory()
{
}
#pragma endregion

#pragma region Instance
SpellsInventory* SpellsInventory::GetInstance()
{
	if (instance.Get() == nullptr)
	{
		instance.Reset(new SpellsInventory);
	}

	return instance.Get();
}
#pragma endregion

#pragma region AimSpells

bool SpellsInventory::IncreaseAimSpellCount() //Called when the player increases the number of equipped aim spells
{
	if (aimSpellsCount + 1 <= maxAimSpellsCount)
	{
		aimSpellsCount++;
		return true;
	}
	return false;
}

bool SpellsInventory::EquipNewAimSpell(const EAimSpells spell_) //Called when the player equips a new aim spell
{
	if (aimSpells.Num() + 1 <= aimSpellsCount)
	{
		aimSpells.Add(spell_);
		aimSpellsCount++;
		return true;
	}
	return false;
}
void SpellsInventory::RemoveAimSpell(const int index_) //Called when the player unequips a new aim spell
{
	if(aimSpells.Num() > 0)
	   aimSpells.RemoveAt(index_);
}

EAimSpells SpellsInventory::UseAimSpell(const int index_, float& currentBP_, bool& increaseHP_, float& spellBaseDamge_,EStatusEffects& status_)
{
	if (index_ <= aimSpells.Num() - 1)
	{
		int cost = GetAimSpellCost(aimSpells[index_]); //Also updates bSpellIncreasesHealth
		//Player can only equip and remove spells which ensures they're always ordered sequentially. The UI should reflect the order correctly whenever the player does any of those actions
		if (currentBP_ >= cost) //Do we have enough BP?
		{
			currentBP_ -= cost;
			//TODO
		//update the variabls passed in by reference
		//spellBaseDamage_ will be filled by the spell's base damage. In NecromancerCharacter.cpp this will be added to character magic stat which will be added to this file
			increaseHP_ = bSpellIncreasesHealth;
			//TODO

			if (bSpellIncreasesHealth)
				bSpellIncreasesHealth = false;

			return aimSpells[index_];
		}
	}
		return EAimSpells::AIMNONE;
}
#pragma endregion

#pragma region BloodSpells

bool SpellsInventory::IncreaseBloodSpellCount()//Called when the player increases the number of equipped blood spells
{
	if (bloodSpellsCount + 1 <= maxBloodSpellsCount)
	{
		bloodSpellsCount++;
		return true;
	}
	return false;
}

bool SpellsInventory::EquipNewBloodSpell(const EBloodSpells spell_) //Called when the player equips a new blood spell
{
	if ( bloodSpells.Num() + 1 <= bloodSpellsCount)
	{
		bloodSpells.Add(spell_);
		bloodSpellsCount++;
		return true;
	}
	return false;
}

void SpellsInventory::RemoveBloodSpell(const int index_) //Called when the player unequips a new blood spell
{
	if(bloodSpells.Num() > 0)
	    bloodSpells.RemoveAt(index_);
}

EBloodSpells SpellsInventory::UseBloodSpell(const int index_, float& currentBP_, bool& increaseHP_, float& spellBaseDamge_, EStatusEffects& status_)
{
	if (index_ <= bloodSpells.Num() - 1)
	{
		int cost = GetBloodSpellCost(bloodSpells[index_]); //Also updates bSpellIncreasesHealth
														 //Player can only equip and remove spells (no swapping) which ensures they're always ordered sequentially. The UI should reflect the order correctly whenever the player does any of those actions
		if (currentBP_ >= cost) //Do we have enough BP?
		{
			currentBP_ -= cost;
			//TODO
		//update the variabls passed in by reference
			increaseHP_ = bSpellIncreasesHealth;
			//TODO

			if (bSpellIncreasesHealth)
				bSpellIncreasesHealth = false;

			return bloodSpells[index_];
		}
	}
	return EBloodSpells::BLOODNONE;
}

#pragma endregion

#pragma region Innate

bool  SpellsInventory::EquipNewInnateSpell(const EInnateSpells spell_)
{
	if (currentInnateSpell == INNATENONE)
	{
		currentInnateSpell = spell_;
		return true;
	}
	return false;
}
void  SpellsInventory::RemoveInnateSpell()
{
	currentInnateSpell = INNATENONE;
}

bool SpellsInventory::UseInnateSpell(float& currentBP_, bool& damagesPlayer_, float& currentHP_ , bool& increaseHP_, float& spellBaseDamage_, EStatusEffects& status_)
{
	if (currentInnateSpell != EInnateSpells::INNATENONE)
	{
		int cost = GetInnateSpellCost(currentInnateSpell, damagesPlayer_);
		if (damagesPlayer_) //If damagesPlayer_ is set true, NecroCharacter should play a takeDamage effect on the player
		{
			if (currentHP_ - cost > 0.0f) //Don't die
			{
				currentHP_ -= cost;
				currentBP_ += cost * 2.0f;
				return false; //False is returned if the spell failed, or if you are not healing. i.e. no need to call conjuror
			}
			else
			{
				damagesPlayer_ = false; //If current health - cost will be less than zero, don't run damage effect
				return false;
			}
		}
		else
		{
			if (currentBP_ >= cost)
			{
				currentBP_ -= cost;
				return true; //Necro Character will call conjuror
			}
		}
	}

	return false;	
}

#pragma endregion

#pragma region SpellCost

float SpellsInventory::GetAimSpellCost(const EAimSpells spell_)
{
	switch (spell_)
	{
	case BLOODSPEAR:
		return 10.0f;
		break;
	case BLOODRAIN:
		return 10.0f;
		break;
	case BLOODSPRAY:
		return 10.0f;
		break;
	case ISEEDEATH:
		return 10.0f;
		break;
	case SERVEINDEATH:
		return 10.0f;
		break;
	case EYESOFBLOOD:
		return 10.0f;
		break;
	case SWARM:
		bSpellIncreasesHealth = true;
		return 10.0f;
		break;
	default:
		return 0.0f;
		break;
	}
	return 0.0f;
}

float SpellsInventory::GetBloodSpellCost(const EBloodSpells spell_)
{
	switch (spell_)
	{
	default:
	case SKELETONHANDS:
		return 10.0f;
		break;
	case BLOODTRAP:
		return 10.0f;
		break;
	case BLOODPYRE:
		return 10.0f;
		break;
	case BLOODBLOAT:
		return 10.0f;
		break;
		return 0.0f;
		break;
	}
}

float SpellsInventory::GetInnateSpellCost(const EInnateSpells spell_, bool& damagesplayer_)
{
	switch (spell_)
	{
	case FLESHISASERVANT:
		damagesplayer_ = true;
	return 10.0f;
	break;
	default:
		return 0.0f;
	}
	return 0.0f;
}

#pragma endregion
