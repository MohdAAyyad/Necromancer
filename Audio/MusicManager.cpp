// Fill out your copyright notice in the Description page of Project Settings.


#include "MusicManager.h"
#include "MusicActor.h"

TUniquePtr<MusicManager, TDefaultDelete<MusicManager>> MusicManager::instance = nullptr;


MusicManager::MusicManager()
{
}

MusicManager::~MusicManager()
{
}

MusicManager* MusicManager::GetInstance()
{
	if (instance.Get() == nullptr)
	{
		instance.Reset(new MusicManager);
	}
	return instance.Get();
}

void  MusicManager::RegisterMusicActor(AMusicActor* musicActor_)
{
	musicActor = musicActor_;
}

void MusicManager::SwtichMusic(int index_)
{
	if (musicActor)
		musicActor->SwitchMusic(index_);
}

void MusicManager::FadeOutCurrentAudio()
{
	if (musicActor)
		musicActor->FadeOutCurrentAudio();
}