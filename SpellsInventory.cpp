// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellsInventory.h"

TUniquePtr<SpellsInventory, TDefaultDelete<SpellsInventory>> SpellsInventory::instance = nullptr;
#pragma region Constructor/Destructon



SpellsInventory::SpellsInventory()
{
	aimSpellsCount = 3;
	bloodSpellsCount = 3;
	maxAimSpellsCount = 4;
	maxBloodSpellsCount = 4;
	currentInnateSpell = EInnateSpells::INNATENONE;

	UnlockAimSpell(EAimSpells::BLOODSHOT);
	UnlockAimSpell(EAimSpells::BLOODROCKET);
	UnlockAimSpell(EAimSpells::BLOODTIMEBOMB);

	UnlockBloodSpell(EBloodSpells::BLOODMIASMA);
	UnlockBloodSpell(EBloodSpells::SERVEINDEATH);
	UnlockBloodSpell(EBloodSpells::BLOODTORNADO);
	UnlockBloodSpell(EBloodSpells::BLOODEXPLOSION);
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

bool SpellsInventory::EquipNewAimSpell(const int index_) //Called when the player equips a new aim spell
{
	if (aimSpells.Num() + 1 <= aimSpellsCount && index_<=unlockedAimSpells.Num()-1 && index_>=0)
	{
		if (!aimSpells.Contains(unlockedAimSpells[index_]))
		{
			aimSpells.Add(unlockedAimSpells[index_]);
			return true;
		}
	}
	return false;
}
void SpellsInventory::RemoveAimSpell(const int index_) //Called when the player unequips a new aim spell
{
	if(aimSpells.Num() > 0 && index_ <= aimSpells.Num() - 1 && index_ >= 0)
	   aimSpells.RemoveAt(index_);
}

EAimSpells SpellsInventory::GetAimSpell(const int index_)
{
	if (index_ <= aimSpells.Num() - 1 && index_ >= 0)
	{
		return aimSpells[index_];
	}
	return EAimSpells::AIMNONE;
}

EAimSpells SpellsInventory::GetAimSpellForTexture(int index_)
{
	if (index_ <= unlockedAimSpells.Num() - 1 && index_ >= 0)
	{
		return unlockedAimSpells[index_];
	}
	return EAimSpells::AIMNONE;
}

EAimSpells SpellsInventory::GetEquippedAimSpellForTexture(int index_)
{
	if (index_ <= aimSpells.Num() - 1 && index_ >= 0)
	{
		return aimSpells[index_];
	}
	return EAimSpells::AIMNONE;
}

void SpellsInventory::UnlockAimSpell(EAimSpells spell_)
{
	unlockedAimSpells.Add(spell_);
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

bool SpellsInventory::EquipNewBloodSpell(const int index_) //Called when the player equips a new blood spell
{
	if ( bloodSpells.Num() + 1 <= bloodSpellsCount && index_ <= unlockedBloodSpells.Num() - 1 && index_ >= 0)
	{
		if (!bloodSpells.Contains(unlockedBloodSpells[index_]))
		{
			bloodSpells.Add(unlockedBloodSpells[index_]);
			return true;
		}
	}
	return false;
}

void SpellsInventory::RemoveBloodSpell(const int index_) //Called when the player unequips a new blood spell
{
	if(bloodSpells.Num() > 0 && index_ <= bloodSpells.Num() - 1 && index_ >= 0)
	    bloodSpells.RemoveAt(index_);
}

EBloodSpells SpellsInventory::GetBloodSpell(const int index_, bool& corpseSpell_)
{
	if (index_ <= bloodSpells.Num() - 1 && index_ >= 0)
	{
		switch (bloodSpells[index_]) //Check if the spell is a corpse spell
		{
		case EBloodSpells::SERVEINDEATH:
			corpseSpell_ = true;
			break;
		case EBloodSpells::BLOODEXPLOSION:
			corpseSpell_ = true;
			break;
		default:
			corpseSpell_ = false;
			break;
		}

		return bloodSpells[index_];
	}
	return EBloodSpells::BLOODNONE;
}

EBloodSpells SpellsInventory::GetBloodSpellForTexture(int index_)
{
	if (index_ <= unlockedBloodSpells.Num() - 1 && index_ >= 0)
	{
		return unlockedBloodSpells[index_];
	}
	return EBloodSpells::BLOODNONE;
}

EBloodSpells SpellsInventory::GetEquippedBloodSpellForTexture(int index_)
{
	if (index_ <= bloodSpells.Num() - 1 && index_ >= 0)
	{
		return bloodSpells[index_];
	}
	return EBloodSpells::BLOODNONE;
}

void SpellsInventory::UnlockBloodSpell(EBloodSpells spell_)
{
	unlockedBloodSpells.Add(spell_);
}
#pragma endregion

#pragma region Innate

bool  SpellsInventory::EquipNewInnateSpell(const int index_)
{
	if (currentInnateSpell == EInnateSpells::INNATENONE && index_<=unlockedInnateSpells.Num()-1 && index_ >= 0)
	{
		currentInnateSpell = unlockedInnateSpells[index_];
		return true;
	}
	return false;
}
void  SpellsInventory::RemoveInnateSpell()
{
	currentInnateSpell = EInnateSpells::INNATENONE;
}

EInnateSpells SpellsInventory::GetInnateSpell()
{
	return currentInnateSpell;
}

EInnateSpells SpellsInventory::GetInnateSpellForTexture(int index_)
{
	if (index_ <= unlockedInnateSpells.Num() - 1 && index_ >= 0)
	{
		return unlockedInnateSpells[index_];
	}
	return EInnateSpells::INNATENONE;
}

EInnateSpells SpellsInventory::GetEquippedInnateSpellForTexture()
{
	return currentInnateSpell;
}

void SpellsInventory::UnlockInnateSpell(EInnateSpells spell_)
{
	unlockedInnateSpells.Add(spell_);
}

#pragma endregion