// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/PlayerUIController.h"

/**
 * 
 */
class NECROMANCER_API EXPManager
{
public:
	~EXPManager();
	EXPManager(const EXPManager&) = delete;
	EXPManager(EXPManager&&) = delete;
	EXPManager& operator=(const EXPManager&) = delete;
	EXPManager& operator=(EXPManager&&) = delete;

private:
	EXPManager();
	static TUniquePtr<EXPManager, TDefaultDelete<EXPManager>> instance;
	int currentEXP;
	int maxEXP;
	int skillPoints;
	UPlayerUIController* uictrl;

public:
	static EXPManager* GetInstance();
	inline int GetCurrentEXP() const { return currentEXP; };
	inline int GetMaxEXP() const { return maxEXP; };
	inline int GetCurrentSkillPoints() const { return skillPoints; };
	bool UpdateCurrentEXP(int expToBeAdded_); //Adds exp to the current amount. Returns true if there's a level up
	inline void UseASkillPoint() { skillPoints--; };
	inline void RegisterUICTRL(UPlayerUIController* uictrl_) { uictrl = uictrl_; };

};
