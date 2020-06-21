// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class NECROMANCER_API MusicManager
{
public:

	~MusicManager();
	MusicManager(const MusicManager&) = delete;
	MusicManager(MusicManager&&) = delete;
	MusicManager& operator=(const MusicManager&) = delete;
	MusicManager& operator=(MusicManager&&) = delete;
protected:
	MusicManager();
	static TUniquePtr<MusicManager, TDefaultDelete<MusicManager>> instance;
	class AMusicActor* musicActor;
public:
	static MusicManager* GetInstance();
	void RegisterMusicActor(class AMusicActor* musicActor_);
	void SwtichMusic(int index_);
	void FadeOutCurrentAudio();
};
