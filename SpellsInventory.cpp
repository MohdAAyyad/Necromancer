// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellsInventory.h"

TUniquePtr<SpellsInventory, TDefaultDelete<SpellsInventory>> SpellsInventory::instance = nullptr;
#pragma region Constructor/Destructon



SpellsInventory::SpellsInventory()
{
	aimSpellsCount = 0;
	bloodSpellsCount = 0;
	maxAimSpellsCount = 4;
	maxBloodSpellsCount = 4;
	currentInnateSpell = EInnateSpells::INNATENONE;

	UnlockBloodSpell(EBloodSpells::SUMMONSKELETON);
	UnlockBloodSpell(EBloodSpells::SERVEINDEATH);
	UnlockBloodSpell(EBloodSpells::BLOODTORNADO);
	UnlockAimSpell(EAimSpells::BLOODROCKET);
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
	if (aimSpells.Num() + 1 <= maxAimSpellsCount)
	{
		if (!aimSpells.Contains(spell_))
		{
			aimSpells.Add(spell_);
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
	if(!unlockedAimSpells.Contains(spell_))
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

bool SpellsInventory::EquipNewBloodSpell(const EBloodSpells spell_) //Called when the player equips a new blood spell
{
	//Check whether you can add a spell (less than you max count, the idea is the player would be able to increase the count with time), and check if the index is within the range of unlocked spells
	if ( bloodSpells.Num() + 1 <= maxBloodSpellsCount)
	{
		if (!bloodSpells.Contains(spell_))
		{
			bloodSpells.Add(spell_);
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
	if (!unlockedBloodSpells.Contains(spell_))
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
	if(!unlockedInnateSpells.Contains(spell_))
		unlockedInnateSpells.Add(spell_);
}

#pragma endregion

#pragma region Names And Descriptions

FString SpellsInventory::GetAimSpellName(EAimSpells spell_)
{
	switch (spell_)
	{
	case EAimSpells::BLOODSHOT:
		return "Blood Shot";
	case EAimSpells::BLOODROCKET:
		return "Blood Rocket";
	case EAimSpells::BLOODTIMEBOMB:
		return "Blood Time Bomb";
	default:
		return "";		
	}
	return "";
}
FString SpellsInventory::GetAimSpellDescription(EAimSpells spell_)
{
	switch (spell_)
	{
	case EAimSpells::BLOODSHOT:
		return "Fires a short range blood splash that deals a lot of damage.";
	case EAimSpells::BLOODROCKET:
		return "Shoots a rocket of blood that explodes on impact.";
	case EAimSpells::BLOODTIMEBOMB:
		return "Shoots a bomb that sticks to its target and explodes \n after a few a seconds.";
	default:
		return "";
	}
	return "";
}

FString SpellsInventory::GetBloodSpellName(EBloodSpells spell_)
{
	switch (spell_)
	{
	case EBloodSpells::BLOODMIASMA:
		return "Blood Miasma";
	case EBloodSpells::BLOODTORNADO:
		return "Blood Tornado";
	case EBloodSpells::SERVEINDEATH:
		return "Serve In Death";
	case EBloodSpells::BLOODEXPLOSION:
		return "Blood Explosion";
	case EBloodSpells::SUMMONSKELETON:
		return "Summon Skeleton";
	default:
		return "";
	}
	return "";
}
FString SpellsInventory::GetBloodSpellDescription(EBloodSpells spell_)
{
	switch (spell_)
	{
	case EBloodSpells::BLOODMIASMA:
		return "Creates a cloud of miasma that poisons enemies who pass through it.";
	case EBloodSpells::BLOODTORNADO:
		return "Creates a tornado of blood wreacking havoc in its path.";
	case EBloodSpells::SERVEINDEATH:
		return "Turns a dead enemy into an undead attacking enemies close to it.";
	case EBloodSpells::BLOODEXPLOSION:
		return "Causes an enemy corpse or an undead ally to explode. \n Can be used to create a chain explosion when targeting undead allies.";
	case EBloodSpells::SUMMONSKELETON:
		return "Summon a skeleton to aid you in the fight.";
	default:
		return "";
	}
	return "";
}

FString SpellsInventory::GetInnateSpellName(EInnateSpells spell_)
{
	switch (spell_)
	{
	case EInnateSpells::FLESHISASERVANT:
		return "Flesh Is A Servant";
	default:
		return "";
	}
	return "";
}
FString SpellsInventory::GetInnateSpellDescription(EInnateSpells spell_)
{
	switch (spell_)
	{
	case EInnateSpells::FLESHISASERVANT:
		return "Flesh Is A Servant";
	default:
		return "Consumes 35% HP and restores 20% BPs";
	}
	return "";
}
#pragma endregion


bool SpellsInventory::IsAimSpellUnlocked(EAimSpells spell_)
{
	return unlockedAimSpells.Contains(spell_);
}
bool SpellsInventory::IsBloodSpellUnlocked(EBloodSpells spell_)
{
	return unlockedBloodSpells.Contains(spell_);
}
bool SpellsInventory::IsInnateSpellUnlocked(EInnateSpells spell_)
{
	return unlockedInnateSpells.Contains(spell_);
}
