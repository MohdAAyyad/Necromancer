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
	~SpellsInventory();
	SpellsInventory(const SpellsInventory&) = delete;
	SpellsInventory(SpellsInventory&&) = delete;
	SpellsInventory& operator=(const SpellsInventory&) = delete;
	SpellsInventory& operator=(SpellsInventory&&) = delete;

	static SpellsInventory* GetInstance();

	void UnlockAimSpell(EAimSpells spell_); //Adds spell to the unlocked array
	bool EquipNewAimSpell(const EAimSpells spell_); //Called when the player equips a new aim spell
	void RemoveAimSpell(const int index_); //Called when the player unequips a new aim spell
	EAimSpells GetAimSpellForTexture(int index_); //Gives UI Controller the spell inside unlockedAimSpells at the specified index
	EAimSpells GetEquippedAimSpellForTexture(int index_); //Gives UI Controller the spell inside aimSpells at the specified index

	int GetAimSpellsNum() { return aimSpells.Num();  };

	void UnlockBloodSpell(EBloodSpells spell_);
	bool EquipNewBloodSpell(const EBloodSpells spell_); //Called when the player equips a new blood spell
	void RemoveBloodSpell(const int index); //Called when the player unequips a new blood spell
	EBloodSpells GetBloodSpell(const int index_, bool& corpseSpell_);
	EBloodSpells GetBloodSpellForTexture(int index_); //Gives UI Controller the spell inside unlockedBloodSpells at the specified index
	EBloodSpells GetEquippedBloodSpellForTexture(int index_); //Gives UI Controller the spell inside bloodSpells at the specified index

	int GetBloodSpellsNum() { return bloodSpells.Num(); };

	bool IncreaseAimSpellCount(); //Called when the player increases the number of equipped aim spells
	bool IncreaseBloodSpellCount();//Called when the player increases the number of equipped blood spells
	EInnateSpells GetInnateSpellForTexture(int index_); //Gives UI Controller the spell inside unlockedInnateSpells at the specified index
	EInnateSpells GetEquippedInnateSpellForTexture(); //Gives UI Controller the current innate spell

	void UnlockInnateSpell(EInnateSpells spell_);

	bool EquipNewInnateSpell(const int index_);
	void RemoveInnateSpell();
	EInnateSpells GetInnateSpell();


	//Names and Description
	FString GetAimSpellName(EAimSpells spell_);
	FString GetAimSpellDescription(EAimSpells spell_);

	FString GetBloodSpellName(EBloodSpells spell_);
	FString GetBloodSpellDescription(EBloodSpells spell_);

	FString GetInnateSpellName(EInnateSpells spell_);
	FString GetInnateSpellDescription(EInnateSpells spell_);


	bool IsAimSpellUnlocked(EAimSpells spell_);
	bool IsBloodSpellUnlocked(EBloodSpells spell_);
	bool IsInnateSpellUnlocked(EInnateSpells spell_);
private:
	SpellsInventory();
	static TUniquePtr<SpellsInventory, TDefaultDelete<SpellsInventory>> instance;

	int aimSpellsCount;
	int maxAimSpellsCount;
	int bloodSpellsCount;
	int maxBloodSpellsCount;
	bool bSpellIncreasesHealth; //If a spell increases health instead 
	TArray<EAimSpells> aimSpells; //Equipped
	TArray<EAimSpells> unlockedAimSpells; //Unlocked
	TArray<EBloodSpells> bloodSpells; //Equipped
	TArray<EBloodSpells> unlockedBloodSpells; //Equipped
	EInnateSpells currentInnateSpell; //Only one innate spell can be equipped at a time
	TArray<EInnateSpells> unlockedInnateSpells;


};
