// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		UTexture2D* crosshairTexture;

	virtual void DrawHUD() override;

public:
	bool bDrawing{ false };

};
