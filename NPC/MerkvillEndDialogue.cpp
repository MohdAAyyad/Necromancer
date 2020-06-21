// Fill out your copyright notice in the Description page of Project Settings.


#include "MerkvillEndDialogue.h"
#include "../AI/Merkvill.h"
#include "../NecromancerGameMode.h"
#include "../Audio/MusicManager.h"
#include "../NecromancerCharacter.h"

//Spawned by merkvill on death around himself
//The credits scene

AMerkvillEndDialogue::AMerkvillEndDialogue():AAutoDialogue()
{
	explosionIndicatorParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Explosion Indicator"));
	explosionIndicatorParticles->SetupAttachment(RootComponent);

	explosionParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Explosion Particles"));
	explosionParticles->SetupAttachment(RootComponent);

	indicatorParticlesIndex = -1;
	explosionParticlesIndex = -1;
	switchMusicIndex = -1;
}

void AMerkvillEndDialogue::BeginPlay()
{
	Super::BeginPlay();
	if (explosionIndicatorParticles)
		explosionIndicatorParticles->DeactivateSystem();

	if (explosionParticles)
		explosionParticles->DeactivateSystem();
}

void AMerkvillEndDialogue::GetNextLineAndName(FString& name_, FString& line_)
{
	if (lineIndex < names[dialogueStageIndex].linesArray.Num() && lineIndex < lines[dialogueStageIndex].linesArray.Num())
	{
		name_ = names[dialogueStageIndex].linesArray[lineIndex];
		line_ = lines[dialogueStageIndex].linesArray[lineIndex];
		if (lineIndex == indicatorParticlesIndex) //Is this the line that invokes the quest? And have we not given the quest yet?
		{
			if (explosionIndicatorParticles)
				explosionIndicatorParticles->ActivateSystem(true);
		}
		else if (lineIndex == explosionParticlesIndex)
		{
			if (explosionParticles)
				explosionParticles->ActivateSystem(true);

			if (explosionIndicatorParticles)
				explosionIndicatorParticles->DeactivateSystem();

			//Play level sequence
			ALevelSequenceActor* sequenceActor;
			sequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), sequence, FMovieSceneSequencePlaybackSettings(), sequenceActor);

			if (sequencePlayer)
				sequencePlayer->Play();
		}
		else if (lineIndex == switchMusicIndex)
		{
			MusicManager::GetInstance()->SwtichMusic(7);
		}
		lineIndex++; //Adding 1 to the line index is not a problem even if we get a new stage. This is because "ResetNPC" is called from the dialogue handler once the conversation ends
	}
}
void AMerkvillEndDialogue::SetMerkvillRef(AMerkvill* merkvill_)
{
	merkvill = merkvill_;

	if (explosionIndicatorParticles)
		explosionIndicatorParticles->SetWorldLocation(merkvill->GetActorLocation());

	if (explosionParticles)
		explosionParticles->SetWorldLocation(merkvill->GetActorLocation());
}

void AMerkvillEndDialogue::ResetNPCDialogue()
{
	ANecromancerGameMode* gameMode = Cast< ANecromancerGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode)
	{
		gameMode->SetLevelName(FName(TEXT("MainMenu")));
		gameMode->SwitchLevel();
	}

	Destroy();
}