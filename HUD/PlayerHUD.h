// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUMG.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

private:
		virtual void BeginPlay() override;
		UTexture2D* crosshairTexture;
	UPROPERTY(EditAnywhere)
		UTexture2D* idleTexture;
	UPROPERTY(EditAnywhere)
		UTexture2D* enemyTexture;
	UPROPERTY(EditAnywhere)
		UTexture2D* bloodPoolTexture;
	UPROPERTY(EditAnywhere, Category = "Widgets")
		TArray<TSubclassOf<UUserWidget>> HUDWdiget;
	UUserWidget* mainMenuWidget;
	UUserWidget* inGameWidget;


	UPROPERTY(EditAnywhere, Category = "Widgets")
		UPlayerUMG* widgett;


public:
	virtual void DrawHUD() override;
	bool bDrawing{ false };
	inline void aimingAtBP() { crosshairTexture = bloodPoolTexture; };
	inline void aimingAtEnemy() { crosshairTexture = enemyTexture; };
	inline void aimingAtIdle(){ crosshairTexture = idleTexture; };
	UFUNCTION(BlueprintCallable)
		void SwitchToInGameWidget();



};
