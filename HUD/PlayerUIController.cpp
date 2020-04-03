// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUIController.h"


// Sets default values for this component's properties
UPlayerUIController::UPlayerUIController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bGameIsPaused = false;
	bSkillTreeMenuIsShown = false;
	spellTextures.Reserve(40);
	// ...
}


// Called when the game starts
void UPlayerUIController::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}

// Called every frame
void UPlayerUIController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerUIController::EquipAimSpell(const int index_)
{
	if (SpellsInventory::GetInstance()->EquipNewAimSpell(index_))
	{

	}
	else
	{
		//Play error sound
	}
}


void UPlayerUIController::EquipBloodSpell(const int index_)
{
	if (SpellsInventory::GetInstance()->EquipNewBloodSpell(index_))
	{

	}
	else
	{
		//Play error sound
	}
}

void UPlayerUIController::EquipInnateSpell(const int index_)
{
	if (SpellsInventory::GetInstance()->EquipNewInnateSpell(index_))
	{

	}
	else
	{
		//Play error sound
	}
}

void UPlayerUIController::PauseGame()
{
	bGameIsPaused = !bGameIsPaused;
}
void UPlayerUIController::SkillTreeMenu()
{
	bSkillTreeMenuIsShown = !bSkillTreeMenuIsShown;
}


float UPlayerUIController::UpdateHealth() const
{
	if(stats)
		return stats->GetHPPercent();

	return 0.0f;
}

float UPlayerUIController::UpdateBlood() const
{
	if (stats)
		return stats->GetBPPercent();
	return 0.0f;
}

#pragma region LookForTextures

UTexture2D*  UPlayerUIController::GetAimTexture(int index_)
{
	return LookForAimTexture(SpellsInventory::GetInstance()->GetAimSpellForTexture(index_));
}

UTexture2D*  UPlayerUIController::GetEquippedAimTexture(int index_)
{
	return LookForAimTexture(SpellsInventory::GetInstance()->GetEquippedAimSpellForTexture(index_));
}

UTexture2D*  UPlayerUIController::GetBloodTexture(int index_)
{
	return LookForBloodTexture(SpellsInventory::GetInstance()->GetBloodSpellForTexture(index_));
}

UTexture2D*  UPlayerUIController::GetEquippedBloodTexture(int index_)
{
	return LookForBloodTexture(SpellsInventory::GetInstance()->GetEquippedBloodSpellForTexture(index_));
}

UTexture2D*  UPlayerUIController::GetInnateTexture(int index_)
{
	return LookForInnateTexture(SpellsInventory::GetInstance()->GetInnateSpellForTexture(index_));
}

UTexture2D*  UPlayerUIController::GetEquippedInnateTexture()
{
	return LookForInnateTexture(SpellsInventory::GetInstance()->GetEquippedInnateSpellForTexture());
}

UTexture2D* UPlayerUIController::LookForAimTexture(EAimSpells spell_)
{
	/*
	spellTextures [0] = NONE
	spellTextures [1-15] = Aim
	spellTextures[16-31] = Blood
	spellTextures [32-40] = Innate

	*/

	switch (spell_)
	{
	case EAimSpells::AIMNONE:
		return spellTextures[0];
	case EAimSpells::BLOODSHOT:
		return spellTextures[1];
	case EAimSpells::BLOODROCKET:
		return spellTextures[2];
	case EAimSpells::BLOODTIMEBOMB:
		return spellTextures[3];
	case EAimSpells::ISEEDEATH:
		return spellTextures[4];
	case EAimSpells::EYESOFBLOOD:
		return spellTextures[6];
	case EAimSpells::SWARM:
		return spellTextures[7];
	default:
		return spellTextures[0];
	}
}
UTexture2D* UPlayerUIController::LookForBloodTexture(EBloodSpells spell_)
{
	switch (spell_)
	{
	case EBloodSpells::BLOODNONE:
		return spellTextures[0];
		break;
	case EBloodSpells::BLOODMIASMA:
		return spellTextures[16];
	case EBloodSpells::SERVEINDEATH:
		return spellTextures[17];
	case EBloodSpells::BLOODEXPLOSION:
		return spellTextures[18];
	case EBloodSpells::BLOODTORNADO:
		return spellTextures[19];
	case EBloodSpells::SUMMONSKELETON:
		return spellTextures[20];
	default:
		return spellTextures[0];
	}
}
UTexture2D* UPlayerUIController::LookForInnateTexture(EInnateSpells spell_)
{
	switch (spell_)
	{
	case EInnateSpells::INNATENONE:
		return spellTextures[0];
	case EInnateSpells::FLESHISASERVANT:
		return spellTextures[32];
	default:
		return spellTextures[0];
	}
}
#pragma endregion



void UPlayerUIController::RemoveAimSpell(int index_)
{
	SpellsInventory::GetInstance()->RemoveAimSpell(index_);
}

void UPlayerUIController::RemoveBloodSpell(int index_)
{
	SpellsInventory::GetInstance()->RemoveBloodSpell(index_);
}

void UPlayerUIController::RemoveInnateSpell()
{
	SpellsInventory::GetInstance()->RemoveInnateSpell();
}

int UPlayerUIController::GetCurrentSkillPoints()
{
	return(EXPManager::GetInstance()->GetCurrentSkillPoints());
}

int UPlayerUIController::GetCurrentEXP()
{
	return(EXPManager::GetInstance()->GetCurrentEXP());
}

int UPlayerUIController::GetEXPToLevelUp()
{
	return(EXPManager::GetInstance()->GetMaxEXP() - EXPManager::GetInstance()->GetCurrentEXP());
}


#pragma region Unlock Spells

void UPlayerUIController::UnlockAimSpell(int index_)
{
	//Make sure we have skill points and we have not unlocked the spell yet
	EAimSpells spell_ = static_cast<EAimSpells>(index_);
	if (EXPManager::GetInstance()->GetCurrentSkillPoints() > 0 && !SpellsInventory::GetInstance()->IsAimSpellUnlocked(spell_))
	{
		SpellsInventory::GetInstance()->UnlockAimSpell(spell_);
		EXPManager::GetInstance()->UseASkillPoint();
	}
}
void UPlayerUIController::UnlockBloodSpell(int index_)
{
	EBloodSpells spell_ = static_cast<EBloodSpells>(index_);
	if (EXPManager::GetInstance()->GetCurrentSkillPoints() > 0 && !SpellsInventory::GetInstance()->IsBloodSpellUnlocked(spell_))
	{
		SpellsInventory::GetInstance()->UnlockBloodSpell(spell_);
		EXPManager::GetInstance()->UseASkillPoint();
	}
}
void UPlayerUIController::UnlockInnateSpell(int index_)
{
	EInnateSpells spell_ = static_cast<EInnateSpells>(index_);
	if (EXPManager::GetInstance()->GetCurrentSkillPoints() > 0 && !SpellsInventory::GetInstance()->IsInnateSpellUnlocked(spell_))
	{
		SpellsInventory::GetInstance()->UnlockInnateSpell(spell_);
		EXPManager::GetInstance()->UseASkillPoint();
	}
}
#pragma endregion


#pragma region Name And Description

FString UPlayerUIController::GetAimName(int index_)
{
	return(SpellsInventory::GetInstance()->GetAimSpellName(SpellsInventory::GetInstance()->GetAimSpellForTexture(index_)));
}

FString UPlayerUIController::GetAimDescription(int index_)
{
	return(SpellsInventory::GetInstance()->GetAimSpellDescription(SpellsInventory::GetInstance()->GetAimSpellForTexture(index_)));
}

FString UPlayerUIController::GetLockedAimName(int index_)
{
	return(SpellsInventory::GetInstance()->GetAimSpellName(static_cast<EAimSpells>(index_)));
}

FString UPlayerUIController::GetLockedAimDescription(int index_)
{
	return(SpellsInventory::GetInstance()->GetAimSpellDescription(static_cast<EAimSpells>(index_)));
}


FString UPlayerUIController::GetBloodName(int index_)
{
	return(SpellsInventory::GetInstance()->GetBloodSpellName(SpellsInventory::GetInstance()->GetBloodSpellForTexture(index_)));
}

FString UPlayerUIController::GetBloodDescription(int index_)
{
	return(SpellsInventory::GetInstance()->GetBloodSpellDescription(SpellsInventory::GetInstance()->GetBloodSpellForTexture(index_)));
}

FString UPlayerUIController::GetLockedBloodName(int index_)
{
	return(SpellsInventory::GetInstance()->GetBloodSpellName(static_cast<EBloodSpells>(index_)));
}

FString UPlayerUIController::GetLockedBloodDescription(int index_)
{
	return(SpellsInventory::GetInstance()->GetBloodSpellDescription(static_cast<EBloodSpells>(index_)));
}


FString UPlayerUIController::GetInnateName(int index_)
{
	return(SpellsInventory::GetInstance()->GetInnateSpellName(SpellsInventory::GetInstance()->GetInnateSpellForTexture(index_)));
}

FString UPlayerUIController::GetInnateDescription(int index_)
{
	return(SpellsInventory::GetInstance()->GetInnateSpellDescription(SpellsInventory::GetInstance()->GetInnateSpellForTexture(index_)));
}

FString UPlayerUIController::GetLockedInnateName(int index_)
{
	return(SpellsInventory::GetInstance()->GetInnateSpellName(static_cast<EInnateSpells>(index_)));
}

FString UPlayerUIController::GetLockedInnateDescription(int index_)
{
	return(SpellsInventory::GetInstance()->GetInnateSpellDescription(static_cast<EInnateSpells>(index_)));
}


#pragma endregion


#pragma region Stats

int UPlayerUIController::GetBaseDamage()
{
	if (stats)
		return stats->GetBaseDamage();

	return 0;
}
int UPlayerUIController::GetBaseMagicDamage()
{
	if (stats)
		return stats->GetBaseMagicDamage();

	return 0;
}
int UPlayerUIController::GetMaxHP()
{
	if (stats)
		return stats->GetMaxHP();

	return 0;
}
int UPlayerUIController::GetMaxBP()
{
	if (stats)
		return stats->GetMaxBP();

	return 0;
}
void UPlayerUIController::AddBaseDamage()
{
	if (GetCurrentSkillPoints() > 0)
	{
		if (stats)
			stats->AddToBaseDamage(5.0f);
		EXPManager::GetInstance()->UseASkillPoint();
	}
}
void UPlayerUIController::AddMagicDamage()
{
	if (GetCurrentSkillPoints() > 0)
	{
		if (stats)
			stats->AddToBaseMagicDamage(5.0f);
		EXPManager::GetInstance()->UseASkillPoint();
	}
}
void UPlayerUIController::AddHP()
{
	if (GetCurrentSkillPoints() > 0)
	{
		if (stats)
			stats->AddToMaxHP(50.0f);
		EXPManager::GetInstance()->UseASkillPoint();
	}
}
void UPlayerUIController::AddBP()
{
	if (GetCurrentSkillPoints() > 0)
	{
		if (stats)
			stats->AddToMaxBP(50.0f);
		EXPManager::GetInstance()->UseASkillPoint();
	}
}

#pragma endregion
