// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUIController.h"


// Sets default values for this component's properties
UPlayerUIController::UPlayerUIController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bGameIsPaused = false;
	hpPercent = 1.0f;
	bloodPercent = 1.0f;
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

float UPlayerUIController::UpdateHealth() const
{
	return hpPercent;
}

float UPlayerUIController::UpdateBlood() const
{
	return bloodPercent;
}

void UPlayerUIController::SetHealthPercentage(float percent_)
{
	hpPercent = percent_;
}

void UPlayerUIController::SetBloodPercentage(float percent_)
{
	bloodPercent = percent_;
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
		break;
	case EAimSpells::BLOODSHOT:
		return spellTextures[1];
		break;
	case EAimSpells::BLOODROCKET:
		return spellTextures[2];
		break;
	case EAimSpells::BLOODTIMEBOMB:
		return spellTextures[3];
		break;
	case EAimSpells::ISEEDEATH:
		return spellTextures[4];
		break;
	case EAimSpells::EYESOFBLOOD:
		return spellTextures[6];
		break;
	case EAimSpells::SWARM:
		return spellTextures[7];
		break;
	default:
		return spellTextures[0];
		break;
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
		break;
	case EInnateSpells::FLESHISASERVANT:
		return spellTextures[32];
		break;
	default:
		return spellTextures[0];
		break;
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



