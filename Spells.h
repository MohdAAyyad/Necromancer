// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "Spells.generated.h"

/**
 * 
 */
UENUM(blueprintType)
enum class EAimSpells : uint8
{
AIMNONE = 0,
BLOODSHOT = 1, // Fires a short range blood splash (similar to a shotgun). High damage, low range
BLOODROCKET = 2, //Shoots a rocket that has splash damage
BLOODTIMEBOMB = 3, //Shoots a sticky bomb that has splash damage
ISEEDEATH = 4, //Enemy targeted by this gets paralyzed with fear for a short amount of time. 
EYESOFBLOOD = 5, //Targeted Enemy becomes blinded for a short amount of time, and they are unable to damage the player.
SWARM = 6 // Player summons mosquitoes that suck the blood from the targeted enemy around. Recovers health.
};

UENUM(blueprintType)
enum class EBloodSpells : uint8
{
	BLOODNONE = 0,
	BLOODMIASMA = 1, // Summons a misama cloud above a blood pool or a corpse
	BLOODTORNADO = 2, //Summon tornado
	SERVEINDEATH = 3, //Turns a dead body into a zombie that attacks the enemy closest to it and lasts for a short amount of time.
	BLOODEXPLOSION = 4, //Makes a corpse or a zombie or a zombie explode
	SUMMONSKELETON = 5 //Summon a skeleton from a blood pool

};

UENUM(blueprintType)
enum class EInnateSpells : uint8
{
	INNATENONE = 0,
	FLESHISASERVANT = 1 //Leah uses her knife to cut herself which lowers HP and restores Blood Points. The amount of Blood Points gained this way can be upgraded in the skill tree, however, the amount of health consumed is always 20% or more of the player’s health.
};
