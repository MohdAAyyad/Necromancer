// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellCheck.h"

TUniquePtr<SpellCheck, TDefaultDelete<SpellCheck>>SpellCheck::instance = nullptr;

SpellCheck::SpellCheck()
{
	bSpellIncreasesHealth = false;
	bSpellIsASummon = false;
}

SpellCheck::~SpellCheck()
{
}

#pragma region Aim

bool SpellCheck::CheckForAimSpell(const EAimSpells spell_, float& currentBP_, bool& increaseHP_, float& spellBaseDamage_, EStatusEffects& status_)
{
	int cost = GetAimSpellCost(spell_, spellBaseDamage_); //Also updates bSpellIncreasesHealth

	if (currentBP_ >= cost) //Do we have enough BP?
	{
		currentBP_ -= cost;
		increaseHP_ = bSpellIncreasesHealth;
		bSpellIncreasesHealth = false; //Reset variable

		return true;
	}

	return false;
}
float SpellCheck::GetAimSpellCost(const EAimSpells spell_, float& spellBaseDamage_)
{
	//Check for cost and add the spell's damage to the player's basic magic damage stat
	switch (spell_)
	{
	case EAimSpells::BLOODSHOT:
		spellBaseDamage_ += 90.0f;
		return 20.0f;
	case EAimSpells::BLOODROCKET:
		spellBaseDamage_ += 50.0f;
		return 40.0f;
	case EAimSpells::BLOODTIMEBOMB:
		spellBaseDamage_ += 50.0f;
		return 35.0f;
	case EAimSpells::ISEEDEATH:
		spellBaseDamage_ = 0.0f;
		return 10.0f;
	case EAimSpells::EYESOFBLOOD:
		spellBaseDamage_ = 0.0f;
		return 10.0f;
	case EAimSpells::SWARM:
		spellBaseDamage_ = 0.0f;
		bSpellIncreasesHealth = true;
		return 10.0f;
	default:
		return 0.0f;
	}
}

#pragma endregion

#pragma region Blood

bool SpellCheck::CheckForBloodSpell(const EBloodSpells spell_, float& currentBP_, bool& isASummon_, float& spellBaseDamage_, EStatusEffects& status_)
{
	int cost = GetBloodSpellCost(spell_, spellBaseDamage_); //Also updates bSpellIsASummon

	if (currentBP_ >= cost) //Do we have enough BP?
	{
		currentBP_ -= cost;
		isASummon_ = bSpellIsASummon;
		bSpellIsASummon = false; //Reset variable

		return true;
	}

	return false;
}
float SpellCheck::GetBloodSpellCost(const EBloodSpells spell_, float& spellBaseDamage_)
{
	switch (spell_)
	{
	case EBloodSpells::BLOODMIASMA:
		spellBaseDamage_ = 0; //Miasma doesn't do any direct damage, only poison damage
		return 10.0f;
	case EBloodSpells::SERVEINDEATH:
		spellBaseDamage_ = 0;
		return 25.0f;
	case EBloodSpells::BLOODEXPLOSION:
		spellBaseDamage_ += 70.0f;
		return 35.0f;
	case EBloodSpells::BLOODTORNADO:
		spellBaseDamage_ += 150.0f;
		return 120.0f;
	case EBloodSpells::SUMMONSKELETON:	
		bSpellIsASummon = true;
		return 40.0f;
		break;
	default:
		return 0.0f;
	}
}
#pragma endregion

#pragma region Innate

bool SpellCheck::CheckForInnateSpell(const EInnateSpells spell_, float& currentBP_, bool& damagesPlayer_, float& currentHP_, bool& increaseHP_, float& spellBaseDamage_, EStatusEffects& status_)
{
		int cost = GetInnateSpellCost(spell_, damagesPlayer_, increaseHP_);
		if (damagesPlayer_) //If damagesPlayer_ is set true, NecroCharacter should play a takeDamage effect on the player
		{
			if (currentHP_ - cost >= 10.0f) //Don't die
			{
				currentHP_ -= cost;
				currentBP_ += cost * 2.0f;
				return false; //False is returned if the spell failed, or if you are affecting HP and not BP. i.e. no need to call conjuror
			}
			else
			{
				damagesPlayer_ = false; //Failed at damaging the player
				return false;
			}
		}
		else if (increaseHP_)
		{
			if (currentBP_>= cost)
			{
				currentBP_ -= cost;
				currentHP_ += cost * 2.0f;
				return false;
			}
			else
			{
				increaseHP_ = false; // Failed at healing the player
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

	return false;
}
float SpellCheck::GetInnateSpellCost(const EInnateSpells spell_, bool& damagesPlayer_, bool& increaseHP_)
{
	switch (spell_)
	{
	case EInnateSpells::FLESHISASERVANT:
		damagesPlayer_ = true;
		increaseHP_ = false;
		return 10.0f;
	default:
		return 0.0f;
	}
}

#pragma endregion



#pragma region Instance

SpellCheck* SpellCheck::GetInstance()
{
	if (!instance)
	{
		instance.Reset(new SpellCheck);
	}

	return instance.Get();
}
#pragma endregion