// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../AI/EnemyBase.h"
#include "EnemyUMG.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API UEnemyUMG : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float hpPercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AEnemyBase* enemyRef;
};
