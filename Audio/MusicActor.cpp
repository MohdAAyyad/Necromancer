// Fill out your copyright notice in the Description page of Project Settings.


#include "MusicActor.h"

// Sets default values
AMusicActor::AMusicActor()
{
	PrimaryActorTick.bCanEverTick = false;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = root;

	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	audioComponent->SetupAttachment(root);
	musicIndex = 0;
	fadeOutTime = 2.5f;
	musicVolume = 0.8f;
}

void AMusicActor::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < music.Num(); i++)
	{
		//Load all the sound files at the beginning to prevent FPS drops when they're loaded
		audioComponent->VolumeMultiplier = 0.0f;
		audioComponent->Sound = music[i];
		audioComponent->Stop();
		audioComponent->Play();
	}
	audioComponent->VolumeMultiplier = musicVolume;

	if (music[musicIndex])
	{
		audioComponent->Sound = music[musicIndex];
		audioComponent->Play();
	}

	MusicManager::GetInstance()->RegisterMusicActor(this);


}

void AMusicActor::SwitchMusic(int musicIndex_)
{
	if (musicIndex_ != musicIndex) //Don't switch if it's the same track
	{
		if (music[musicIndex_])
		{
			musicIndex = musicIndex_;
			if (audioComponent->IsPlaying()) //If playing fade out first
			{
				audioComponent->FadeOut(fadeOutTime, 0.0f);
				GetWorld()->GetTimerManager().SetTimer(fadeOutTimeHandle, this, &AMusicActor::PlayNextPiece, fadeOutTime, false);
			}
			else //Otherwise, start playing
			{
				PlayNextPiece();
			}

		}
	}
}
void AMusicActor::PlayNextPiece()
{
	if (music[musicIndex])
	{
		audioComponent->Sound = music[musicIndex];
		audioComponent->Play();
	}
}

void AMusicActor::FadeOutCurrentAudio()
{
	audioComponent->FadeOut(8.0f, 0.0f);
}
