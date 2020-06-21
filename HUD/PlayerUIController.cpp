// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUIController.h"
#include "TimerManager.h"
#include "../EXPManager.h"
#include "../QuestManager.h"


// Sets default values for this component's properties
UPlayerUIController::UPlayerUIController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bGameIsPaused = false;
	bSkillTreeMenuIsShown = false;
	bInDialogue = false;
	bQuestMenu = false;
	spellTextures.Reserve(40);
	highlightedSpellName = false;
	highlightedSpellDescription = false;
	highlightedSpellName = "";
	highlightedSpellDescription = "";
	shownQuest = 0;
	timeToRemoveScreenPrompt = 2.5f;
	numOfQuests = 0;
	numOfCompletedQuests = 0;
	playCompletedPrompt = false;
	playNewPrompt = false;

	// ...
}


// Called when the game starts
void UPlayerUIController::BeginPlay()
{
	Super::BeginPlay();
	numOfCompletedQuests = QuestManager::GetInstance()->GetCompletedQuestsCount();
	numOfQuests = QuestManager::GetInstance()->GetQuestsCount();
	EXPManager::GetInstance()->RegisterUICTRL(this);
	QuestManager::GetInstance()->RegisterUICTRL(this);
	// ...
	
}

// Called every frame
void UPlayerUIController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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
	return LookForAimTexture(static_cast<EAimSpells>(index_));
}

UTexture2D*  UPlayerUIController::GetEquippedAimTexture(int index_)
{
	return LookForAimTexture(SpellsInventory::GetInstance()->GetEquippedAimSpellForTexture(index_));
}

UTexture2D*  UPlayerUIController::GetBloodTexture(int index_)
{
	return LookForBloodTexture(static_cast<EBloodSpells>(index_));
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
	if (SpellsInventory::GetInstance()->IsBloodSpellLockedBehindAQuest(spell_)) //Check if the spell is locked behind a quest first
	{
		return spellTextures[0];
	}
	else
	{
		switch (spell_)
		{
		case EBloodSpells::BLOODNONE:
			return spellTextures[0];
		case EBloodSpells::BLOODMIASMA:
			return spellTextures[16];
		case EBloodSpells::SERVEINDEATH:
			return spellTextures[17];
		case EBloodSpells::BLOODEXPLOSION: //Quest spell, will be hidden until unlocked
			return spellTextures[18];
		case EBloodSpells::BLOODTORNADO:
			return spellTextures[19];
		case EBloodSpells::SUMMONSKELETON:
			return spellTextures[20];
		default:
			return spellTextures[0];
		}
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


int UPlayerUIController::GetEquippedAimNum()
{
	return (SpellsInventory::GetInstance()->GetAimSpellsNum());
}


int UPlayerUIController::GetEquippedBloodNum()
{
	return (SpellsInventory::GetInstance()->GetBloodSpellsNum());
}


#pragma region Unlock Spells

void UPlayerUIController::UnlockAimSpell(int index_)
{
	//Make sure we have skill points and we have not unlocked the spell yet
	EAimSpells spell_ = static_cast<EAimSpells>(index_);
	if (!SpellsInventory::GetInstance()->IsAimSpellUnlocked(spell_)) //If we have yet to unlock the spell, check if we have enough skill points and unlock it, then equip it
	{
		if (EXPManager::GetInstance()->GetCurrentSkillPoints() > 0)
		{
			SpellsInventory::GetInstance()->UnlockAimSpell(spell_);
			SpellsInventory::GetInstance()->EquipNewAimSpell(spell_);
			EXPManager::GetInstance()->UseASkillPoint();
		}
	}
	else //We have already unlocked the spell, that means we want to equip it
	{
		SpellsInventory::GetInstance()->EquipNewAimSpell(spell_);
	}
}
void UPlayerUIController::UnlockBloodSpell(int index_)
{
	EBloodSpells spell_ = static_cast<EBloodSpells>(index_);
	if (!SpellsInventory::GetInstance()->IsBloodSpellLockedBehindAQuest(spell_))
	{
		if (!SpellsInventory::GetInstance()->IsBloodSpellUnlocked(spell_))
		{
			if (EXPManager::GetInstance()->GetCurrentSkillPoints() > 0)
			{
				SpellsInventory::GetInstance()->UnlockBloodSpell(spell_);
				SpellsInventory::GetInstance()->EquipNewBloodSpell(spell_);
				EXPManager::GetInstance()->UseASkillPoint();
			}
		}
		else
		{
			SpellsInventory::GetInstance()->EquipNewBloodSpell(spell_);
		}
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

bool UPlayerUIController::IsAimSpellUnlocked(int index_)
{
	EAimSpells spell_ = static_cast<EAimSpells>(index_);
	return (SpellsInventory::GetInstance()->IsAimSpellUnlocked(spell_));
}

bool UPlayerUIController::IsBloodSpellUnlocked(int index_)
{
	EBloodSpells spell_ = static_cast<EBloodSpells>(index_);
	return (SpellsInventory::GetInstance()->IsBloodSpellUnlocked(spell_));
}
#pragma endregion


#pragma region Name And Description

FString UPlayerUIController::GetEquippedAimName(int index_)
{
	return(SpellsInventory::GetInstance()->GetAimSpellName(SpellsInventory::GetInstance()->GetEquippedAimSpellForTexture(index_)));
}

FString UPlayerUIController::GetEquippedAimDescription(int index_)
{
	return(SpellsInventory::GetInstance()->GetAimSpellDescription(SpellsInventory::GetInstance()->GetEquippedAimSpellForTexture(index_)));
}

FString UPlayerUIController::GetLockedAimName(int index_)
{
	return(SpellsInventory::GetInstance()->GetAimSpellName(static_cast<EAimSpells>(index_)));
}

FString UPlayerUIController::GetLockedAimDescription(int index_)
{
	return(SpellsInventory::GetInstance()->GetAimSpellDescription(static_cast<EAimSpells>(index_)));
}


FString UPlayerUIController::GetEquippedBloodName(int index_)
{
	return(SpellsInventory::GetInstance()->GetBloodSpellName(SpellsInventory::GetInstance()->GetEquippedBloodSpellForTexture(index_)));
}

FString UPlayerUIController::GetEquippedBloodDescription(int index_)
{
	return(SpellsInventory::GetInstance()->GetBloodSpellDescription(SpellsInventory::GetInstance()->GetEquippedBloodSpellForTexture(index_)));
}

FString UPlayerUIController::GetLockedBloodName(int index_)
{
	EBloodSpells spell_ = static_cast<EBloodSpells>(index_);

	if (!SpellsInventory::GetInstance()->IsBloodSpellLockedBehindAQuest(spell_)) //If the spell is not locked behind a quest, no worries
	{
		return(SpellsInventory::GetInstance()->GetBloodSpellName(spell_));
	}
	return ""; //Otherwise, return an empty string
}

FString UPlayerUIController::GetLockedBloodDescription(int index_)
{
	EBloodSpells spell_ = static_cast<EBloodSpells>(index_);

	if (!SpellsInventory::GetInstance()->IsBloodSpellLockedBehindAQuest(spell_)) //If the spell is not locked behind a quest, no worries
	{
		return(SpellsInventory::GetInstance()->GetBloodSpellDescription(spell_));
	}
	return "";
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

void UPlayerUIController::SetName(FString name_)
{
	highlightedSpellName = name_;
}

void UPlayerUIController::SetDescription(FString description_)
{
	highlightedSpellDescription = description_;
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


#pragma region Dialogue

FString UPlayerUIController::GetDialogue()
{
	return dialogue;
}

FString UPlayerUIController::GetName()
{
	return dialogueName;
}

void UPlayerUIController::AppendNextChar(TCHAR nchar_)
{
	if (!bInDialogue)
		bInDialogue = true;
	dialogue += nchar_;
}

void UPlayerUIController::ClearDialogue()
{
	dialogue = "";
	dialogueName = "";
}
void UPlayerUIController::EndDialogue()
{
	dialogueName = "";
	dialogue = "";
	bInDialogue = false;
}

void UPlayerUIController::SetEntireDialogue(FString dialogue_)
{
	dialogue = dialogue_;
}
void UPlayerUIController::SetDialogueName(FString name)
{
	dialogueName = name;
}
#pragma endregion


#pragma region Quests

FString UPlayerUIController::GetQuestName(int index_)
{
	return QuestManager::GetInstance()->GetQuestName(index_);
}

FString UPlayerUIController::GetQuestDescription()
{
	return QuestManager::GetInstance()->GetQuestDescription(shownQuest);
}

FString UPlayerUIController::GetActiveQuestName()
{
	return QuestManager::GetInstance()->GetActiveQuestName();
}

void UPlayerUIController::SetActiveQuest()
{
	QuestManager::GetInstance()->SetActiveQuest(shownQuest);
}

bool UPlayerUIController::GetIsQuestCompleted(int index_)
{
	return QuestManager::GetInstance()->IsQuestCompleted(index_);
}

void UPlayerUIController::QuestMenu()
{
	bQuestMenu = !bQuestMenu;
}

void UPlayerUIController::SetShownQuest(int index_)
{
	shownQuest = index_;
}
bool UPlayerUIController::IsThisTheShownQuest()
{
	return shownQuest == QuestManager::GetInstance()->GetActiveQuestIndex();
}
bool UPlayerUIController::GetIsShownQuestCompleted()
{
	return GetIsQuestCompleted(shownQuest);
}

void UPlayerUIController::PlayCompletedQuestPrompt()
{
		playCompletedPrompt = true; //Enable the prompt
		GetWorld()->GetTimerManager().SetTimer(timeToRemoveScreenPromptHandle, this, &UPlayerUIController::ResetPlayQuestPrompt, timeToRemoveScreenPrompt, false);
}

bool UPlayerUIController::GetCompletedQuestPrompt()
{
	return playCompletedPrompt;
}

void UPlayerUIController::PlayNewQuestPrompt()
{
	playNewPrompt = true; //Enable the prompt
	GetWorld()->GetTimerManager().SetTimer(timeToRemoveScreenPromptHandle, this, &UPlayerUIController::ResetPlayQuestPrompt, timeToRemoveScreenPrompt, false);
}

bool UPlayerUIController::GetNewQuestPrompt()
{
	return playNewPrompt;
}

void  UPlayerUIController::PlayUpdatedQuestPrompt()
{
	playUpdatedPrompt = true;
	GetWorld()->GetTimerManager().SetTimer(timeToRemoveScreenPromptHandle, this, &UPlayerUIController::ResetPlayQuestPrompt, timeToRemoveScreenPrompt, false);
}

bool  UPlayerUIController::GetUpdatedQuestPrompt()
{
	return playUpdatedPrompt;
}

void UPlayerUIController::ResetPlayQuestPrompt()
{
	playCompletedPrompt = false;
	playNewPrompt = false;
	playUpdatedPrompt = false;
}
#pragma endregion


void UPlayerUIController::GoBack()
{
	bSkillTreeMenuIsShown = false;
	bQuestMenu = false;
}

#pragma region LevelUp
void UPlayerUIController::PlayLevelUpPrompt()
{
	bHasLeveledUp = true;
	GetWorld()->GetTimerManager().SetTimer(timeToRemoveLevelUpPromptHandle, this, &UPlayerUIController::ResetHasLeveledUp, timeToRemoveScreenPrompt, false);

}
bool UPlayerUIController::GetHasLeveledUp()
{
	return bHasLeveledUp;
}
void UPlayerUIController::ResetHasLeveledUp()
{
	bHasLeveledUp = false;
}
#pragma endregion