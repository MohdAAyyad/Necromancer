// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "MusicManager.h"
#include "../TimerManager.h"
#include "MusicActor.generated.h"

UCLASS()
class NECROMANCER_API AMusicActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMusicActor();

protected:
	void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Root")
		USceneComponent* root;
	UPROPERTY(EditAnywhere, Category = "AudioComponent")
		UAudioComponent* audioComponent;
	UPROPERTY(EditAnywhere, Category = "AudioComponent")
		TArray<USoundBase*> music;
	UPROPERTY(EditAnywhere, Category = "AudioComponent")
		int musicIndex;
	UPROPERTY(EditAnywhere, Category = "AudioComponent")
		float musicVolume;
	
	FTimerHandle fadeOutTimeHandle;
	float fadeOutTime;

public:
	void SwitchMusic(int musicIndex_);
	void PlayNextPiece();
	void FadeOutCurrentAudio();

};
