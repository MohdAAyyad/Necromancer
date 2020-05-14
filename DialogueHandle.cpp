// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueHandle.h"
#include "HUD/PlayerUIController.h"
#include "NPC/NPC.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UDialogueHandle::UDialogueHandle()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	uictrl = nullptr;
	timeToNextChar = 0.05f;
	bWithinRangeOfAnNPC = false;
	bEngagedInADialogue = false;
	displayingDialogue = false;
	// ...
}


void UDialogueHandle::BeginPlay()
{
	Super::BeginPlay();
}

void UDialogueHandle::SetNextDialogue()
{
	if (npc)
	{
		npc->GetNextLineAndName(nextName, nextDialogue);
		if(uictrl)
			uictrl->SetDialogueName(nextName);
	}
}

void UDialogueHandle::UpdateDialogueUI()
{
	//Read the dialogue one character at a time
	if (nextDialogueCharIndex < nextDialogueChars.Num() && displayingDialogue)
	{
		if (uictrl)
			uictrl->AppendNextChar(nextDialogueChars[nextDialogueCharIndex]);
		nextDialogueCharIndex++;
		//Call the next character recursively
		GetWorld()->GetTimerManager().SetTimer(timeToNextCharHandle, this, &UDialogueHandle::UpdateDialogueUI, timeToNextChar, false);
	}
	else
	{
		displayingDialogue = false; //Next C will move to the next line
	}
}

void UDialogueHandle::SetUICTRL(UPlayerUIController* uictrl_)
{
	uictrl = uictrl_;
}

void UDialogueHandle::ReadNextLine()
{
		if (nextDialogue != "endl") //endl is the end of the conversation
		{
			//Transform the next piece of dialogue into a char array
			nextDialogueChars.Empty(); //Clear the char array first
			nextDialogueChars = nextDialogue.GetCharArray();
			nextDialogueCharIndex = 0;
			displayingDialogue = true; //We are displaying the characters now
			uictrl->ClearDialogue(); //Clear the current name and lines
			if (uictrl)
				uictrl->SetDialogueName(nextName); //Update the name
			UpdateDialogueUI(); //Update the lines


		}
		else
		{
			//Clear the array, and tell the UICTRL that we're done
			nextDialogueChars.Empty();
			nextDialogueCharIndex = 0;
			displayingDialogue = false;
			bEngagedInADialogue = false;
			if (uictrl)
				uictrl->EndDialogue();	
			if (npc)
				npc->ResetNPCDialogue(); //Get ready to be talked to again
		}
}

void UDialogueHandle::ProcessInput()
{
	//If within range of NPC, initiate dialogue
	if (bWithinRangeOfAnNPC)
	{
		if(!bEngagedInADialogue)
			bEngagedInADialogue = true;

		if (!displayingDialogue)
		{
			//Not displaying the dialogue yet, tell the NPC to give you the next line
			SetNextDialogue();
			ReadNextLine();
		}
		else //Show all the current lines
		{
			if (uictrl)
				uictrl->SetEntireDialogue(nextDialogue);
			displayingDialogue = false;
		}
	}
}


void  UDialogueHandle::DialogueOverlap
(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ != nullptr && overlappedComponent_ != nullptr)
	{
		//Check if other actor is an NPC
		npc = Cast<ANPC>(otherActor_);
		if (npc)
		{
			//then flip the bWithinRangeOfAnNPC
			bWithinRangeOfAnNPC = true;
		}
	}
}

void UDialogueHandle::EndDialogueOverlap
(UPrimitiveComponent* overlappedComp_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_)
{
	if (otherActor_ != nullptr && otherComp_ != nullptr)
	{
		//TODO 
		npc = Cast<ANPC>(otherActor_);
		if (npc)
		{
			//then flip the bWithinRangeOfAnNPC
			bWithinRangeOfAnNPC = false;
			bEngagedInADialogue = false;
			displayingDialogue = false;
			npc = nullptr;
		}
	}
}