// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MusicManager.h"
#include "Components/BoxComponent.h"
#include "EndBattleMusic.generated.h"

UCLASS()
class NECROMANCER_API AEndBattleMusic : public AActor
{
	GENERATED_BODY()
	
public:	
	AEndBattleMusic();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Root")
		USceneComponent* root;
	UPROPERTY(EditAnywhere, Category = "Box")
		UBoxComponent* box;
	UPROPERTY(EditAnywhere, Category = "Music")
		int playThisMusicNext;
	UPROPERTY(EditAnywhere, Category = "Music")
		bool bGetsDestroyedAfterSwitch; //Some themes like character themes should not be destroyed after interaction
	UFUNCTION()
		void MusicOverlap
		(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult &SweepResult);

};
