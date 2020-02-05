// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "Spells.generated.h"

/**
 * 
 */
UENUM(blueprintType)
enum EAimSpells
{
AIMNONE,
BLOODSPEAR, // Conjures a medium range spear of blood that can damage two enemies in a row and ignores cover.
BLOODRAIN, //Shoots a continuous stream of Blood Particles where the player is aiming. Player cannot leave aim mode while this is running.
BLOODSPRAY, //Targeted enemy will explode upon death damaging nearby enemies and creating a large untainted blood pool.
ISEEDEATH, //Enemy targeted by this gets paralyzed with fear for a short amount of time.
SERVEINDEATH, //Turns a dead body into a zombie that attacks the enemy closest to it and lasts for a short amount of time.
EYESOFBLOOD, //Targeted Enemy becomes blinded for a short amount of time, and they are unable to damage the player.
SWARM // Player summons mosquitoes that suck the blood from the targeted enemy around. Recovers health.
};

UENUM(blueprintType)
enum EBloodSpells
{
	BLOODNONE,
	SKELETONHANDS, // Summons skeleton hands in the blood pool that stop enemies in their tracks for a duration of time.
	BLOODTRAP, // Turns blood pools into acid damaging enemies that pass through them.
	BLOODPYRE, //Turns blood pools into revolving pyres that move around damaging enemies.
	BLOODBLOAT, //Turns the blood pool into a bloated creature that explodes near enemies or after a certain amount of time has passed creating a larger TAINTED blood pool.
};

UENUM(blueprintType)
enum EInnateSpells
{
	INNATENONE,
	FLESHISASERVANT //Leah uses her knife to cut herself which lowers HP and restores Blood Points. The amount of Blood Points gained this way can be upgraded in the skill tree, however, the amount of health consumed is always 20% or more of the player’s health.
};
