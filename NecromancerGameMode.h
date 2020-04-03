// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NecromancerGameMode.generated.h"

UCLASS(minimalapi)
class ANecromancerGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	static FVector currentCheckpoint;
	UPROPERTY(EditAnywhere)
	FName currentLevelName;
public:
	ANecromancerGameMode();
	void SetNewCheckpoint(FVector checkpointLoc_);
	void RespawnPlayer();
	void SetLevelName(FName level_);
	FVector GetStartingPosition();
};



