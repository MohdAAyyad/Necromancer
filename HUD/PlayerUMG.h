// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUIController.h"
#include "PlayerUMG.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API UPlayerUMG : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPlayerUIController* uictrl;

public:
	void SetUICTRL(UPlayerUIController* uictrl_) { uictrl = uictrl_; };
	
};
