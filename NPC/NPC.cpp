// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	lineIndex = 0;
	dialogueStageIndex = 0;
	dialogueBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Dialogue box collision"));
	dialogueBox->SetCollisionProfileName("Dialogue");
	dialogueBox->SetGenerateOverlapEvents(true);
	dialogueBox->SetupAttachment(RootComponent);
	bWithinRangeOfPlayer = false;
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANPC::GetNextLineAndName(FString& name_, FString& line_)
{
	if (lineIndex < names[dialogueStageIndex].linesArray.Num() && lineIndex < lines[dialogueStageIndex].linesArray.Num())
	{
		name_ = names[dialogueStageIndex].linesArray[lineIndex];
		line_ = lines[dialogueStageIndex].linesArray[lineIndex];
		lineIndex++;
	}
}

void ANPC::ResetNPCDialogue()
{
	lineIndex = 0;
	if (dialogueStageIndex == 2) //If Quest has been given, done, and the NPC has reacted to the success, move on to stage 3
		dialogueStageIndex = 3;
}

