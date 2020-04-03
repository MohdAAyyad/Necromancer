// Fill out your copyright notice in the Description page of Project Settings.


#include "EXPManager.h"

TUniquePtr<EXPManager, TDefaultDelete<EXPManager>> EXPManager::instance = nullptr;

EXPManager::EXPManager()
{
	//TODO
	//Update the values from save file
	//TODO
	currentEXP = 0;
	maxEXP = 200;
	skillPoints = 0;
}

EXPManager::~EXPManager()
{
}

EXPManager* EXPManager::GetInstance()
{
	if (instance.Get() == nullptr)
	{
		instance.Reset(new EXPManager);
	}
	return instance.Get();
}

bool EXPManager::UpdateCurrentEXP(int expToBeAdded_)
{
	currentEXP += expToBeAdded_; //Increase EXP
	if (currentEXP >= maxEXP) //Level up!
	{
		currentEXP -= maxEXP;
		maxEXP *= 2; //Double needed EXP
		skillPoints += 2; //Get 2 skill points
		if (currentEXP >= maxEXP)
		{
			UpdateCurrentEXP(0); //If the current exp is still more than the max exp, then level up again
		}
		return true; //Has leveled up
	}
	return false; //No level up
}
