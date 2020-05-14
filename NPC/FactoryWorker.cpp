// Fill out your copyright notice in the Description page of Project Settings.


#include "FactoryWorker.h"
#include "Components/CapsuleComponent.h"


AFactoryWorker::AFactoryWorker():AQuestNPC()
{
	questDescription = "The Steel Army attacked the factory and killed the scouts.\nThey obtained a scroll that allows them to reach\nBlood Haven. Find and retrieve the scroll.";
}

void AFactoryWorker::BeginPlay()
{
	Super::BeginPlay();
	animInstance = Cast<UFactoryWorkerAnimInstance>(GetMesh()->GetAnimInstance());
}
void AFactoryWorker::GetNextLineAndName(FString& name_, FString& line_)
{
	Super::GetNextLineAndName(name_, line_);
	if(lineIndex == 14) //Dies at line 12
		if (animInstance) 
			animInstance->bDead = true;
}
void AFactoryWorker::ResetNPCDialogue()
{
	Super::ResetNPCDialogue();
	dialogueBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); //Can't talk to you anymore
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}