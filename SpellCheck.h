// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>
#include "Spells.h"
#include "StatusEffects.h"
#include "CoreMinimal.h"

/**
 * 
 */
class NECROMANCER_API SpellCheck
{
public:
	~SpellCheck();

	static SpellCheck* GetInstance();
	SpellCheck(const SpellCheck&) = delete;
	SpellCheck(SpellCheck&&) = delete;
	SpellCheck& operator=(const SpellCheck&) = delete;
	SpellCheck& operator=(SpellCheck&&) = delete;

	bool bSpellIncreasesHealth; //If a spell increases health instead 
	bool bSpellIsASummon;

	bool CheckForAimSpell(const EAimSpells spell_, float& currentBP_, bool& increaseHP_, float& spellBaseDamage_, EStatusEffects& status_);
	bool CheckForBloodSpell(const EBloodSpells spell_, float& currentBP_, bool& isASummon_, float& spellBaseDamage_, EStatusEffects& status_);
	bool CheckForInnateSpell(const EInnateSpells spell_, float& currentBP_, bool& damagesPlayer_, float& currentHP_, bool& increaseHP_, float& spellBaseDamage_, EStatusEffects& status_);

	float GetBloodSpellCost(const EBloodSpells spell_, float& damage_); //Get BP cost of the spell //Also adds player magic base damage to the spells damage
	float GetAimSpellCost(const EAimSpells spell_, float& damage_); //Get BP cost of the spell
	float GetInnateSpellCost(const EInnateSpells spell_, bool& damagesPlayer_, bool& increaseHP_); //Get BP cost of the spell



private:
	SpellCheck();
	static TUniquePtr<SpellCheck, TDefaultDelete<SpellCheck>> instance;
};
