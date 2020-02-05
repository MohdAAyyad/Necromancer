// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "StatusEffects.generated.h"

/**
 * 
 */
UENUM()
enum EStatusEffects
{
	NONE,
	BLEEDING,
	BURNING,
	POISONED,
	PARALYZED,
	BLINDED
};

UENUM()
enum EStatusDuration //Scales with player stats
{
	MIN,
	MOD,
	MAX
};
