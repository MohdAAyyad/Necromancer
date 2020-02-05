// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spells.h"
#include "StatusEffects.h"

/**
 * 
 */
class NECROMANCER_API SpellsInventory
{
public:
	SpellsInventory();
	~SpellsInventory();
	SpellsInventory(const SpellsInventory&) = delete;
	SpellsInventory(SpellsInventory&&) = delete;
	SpellsInventory& operator=(const SpellsInventory&) = delete;
	SpellsInventory& operator=(SpellsInventory&&) = delete;

	static SpellsInventory* GetInstance();

	bool EquipNewAimSpell(const EAimSpells spell_); //Called when the player equips a new aim spell
	void RemoveAimSpell(const int index_); //Called when the player unequips a new aim spell
	EAimSpells UseAimSpell(const int index_, float& currentBP_, bool& increaseHP_, float& spellBaseDamage_, EStatusEffects& status_); //Called with player input. Calls SpellConjuror class
	bool EquipNewBloodSpell(const EBloodSpells spell_); //Called when the player equips a new blood spell
	void RemoveBloodSpell(const int index); //Called when the player unequips a new blood spell
	EBloodSpells UseBloodSpell(const int index_, float& currentBP_, bool& increaseHP_, float& spellBaseDamage_, EStatusEffects& status_); //Called with player input. Calls SpellConjuror class
	bool IncreaseAimSpellCount(); //Called when the player increases the number of equipped aim spells
	bool IncreaseBloodSpellCount();//Called when the player increases the number of equipped blood spells

	bool EquipNewInnateSpell(const EInnateSpells spell_);
	void RemoveInnateSpell();
	bool UseInnateSpell(float& currentBP_, bool& damagesPlayer_ , float& currentHP_ , bool& increaseHP_, float& spellBaseDamage_, EStatusEffects& status_);
private:
	static TUniquePtr<SpellsInventory, TDefaultDelete<SpellsInventory>> instance;

	int aimSpellsCount;
	int maxAimSpellsCount;
	int bloodSpellsCount;
	int maxBloodSpellsCount;
	bool bSpellIncreasesHealth; //If a spell increases health instead 
	TArray<EAimSpells> aimSpells;
	TArray<EBloodSpells> bloodSpells;
	EInnateSpells currentInnateSpell; //Only one innate spell can be equipped at a time
	float GetBloodSpellCost(const EBloodSpells spell_); //Get BP cost of the spell
	float GetAimSpellCost(const EAimSpells spell_); //Get BP cost of the spell
	float GetInnateSpellCost(const EInnateSpells spell_, bool& damagesPlayer_); //Get BP cost of the spell



};
