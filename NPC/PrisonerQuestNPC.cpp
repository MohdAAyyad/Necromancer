// Fill out your copyright notice in the Description page of Project Settings.


#include "PrisonerQuestNPC.h"
#include "../AimProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"

APrisonerQuestNPC::APrisonerQuestNPC():AQuestNPC()
{
	prisonerBT = CreateDefaultSubobject<UBehaviorTree>(TEXT("BehaviorTree"));
	damageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Box collision"));
	damageBox->SetGenerateOverlapEvents(true);
	damageBox->SetCollisionProfileName("AliveEnemy");
	damageBox->SetupAttachment(RootComponent);
	zombifyDuration = 1.0f;
	aiController = nullptr;
	destinationParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("destinationParticles"));
	destinationParticles->SetupAttachment(RootComponent);
	bDead = true; //So that the prisoner wouldn't get hit until the door is broken
	questDescription = "Leah found the prisoner and destroyed the cage.\n It seems she has a son waiting for her at home.\n Leah pondered whether to free or kill the prisoner\nas she was instructed.";
	//End Quest description in case the player makes the decision to kill the prisoner. The quest description for letting her live is handled by the QuestAutoDialogue
	endQuestDescription = "As instructed by Kay-Hen, Leah killed the prisoner.\nShe turned undead and ran off to her new master.\n Leah learned a new spell from Kay-Hen, but at what cost?";
}

void APrisonerQuestNPC::BeginPlay()
{
	Super::BeginPlay();
	if (damageBox)
		damageBox->OnComponentBeginOverlap.AddDynamic(this, &APrisonerQuestNPC::DestinationOverlap);
	if(dialogueBox)
		dialogueBox->OnComponentBeginOverlap.AddDynamic(this, &APrisonerQuestNPC::DestinationOverlap);

	if (destinationParticles)
		destinationParticles->DeactivateSystem();

	aiController = Cast<AEnemyAIController>(GetController());

	animInstance = Cast<UPrisonerAnimInstance>(GetMesh()->GetAnimInstance());
}

void APrisonerQuestNPC::GetNextLineAndName(FString& name_, FString& line_)
{
	if (QuestManager::GetInstance()->GetQuestIndex(questName) != -1) //Check first if we have obtained the quest
	{
		Super::GetNextLineAndName(name_, line_); //If yes, update the lines normally
	}
	else
	{
		line_ = "endl"; //Otherwise, don't initiate the conversation at all
	}
}

void APrisonerQuestNPC::QuestUpdate()
{
	//Called by the prison wall when the player breaks it
	dialogueStageIndex = 1; //Stage 1 --> Prisoner thanks the player for getting them out
	QuestManager::GetInstance()->AddQuest(questName,questDescription); //Update the description of the quest
	bDead = false; //Can be hit
}
void APrisonerQuestNPC::Death()
{
	if (animInstance)
		animInstance->SetDead();	

	//Automatically turns into a zombie
	GetWorld()->GetTimerManager().SetTimer(zombifyTimerHandle, this, &APrisonerQuestNPC::Zombify, zombifyDuration, false);
}
void APrisonerQuestNPC::Zombify()
{
	dialogueStageIndex = 2; //Move to stage 2 //stage 2 will consist of one line "endl" so that the conversation is not initiated
	if (animInstance)
		animInstance->SetZombie();		
}
void APrisonerQuestNPC::EndZombify()
{
	if (destinationParticles)
		destinationParticles->ActivateSystem(true);

	GetCharacterMovement()->Velocity = FVector::ZeroVector; //Stop (Mainly to update the animation)

	//Reached the destination, allow the particles to play for a little while, then destroy the object
	GetWorld()->GetTimerManager().SetTimer(zombifyTimerHandle, this, &APrisonerQuestNPC::DestroySelf, zombifyDuration, false);
}

void APrisonerQuestNPC::DestroySelf()
{
	Destroy();
}

void APrisonerQuestNPC::ActivateZombie()
{
	//NPC turns into a zombie and runs to the destination
	if (aiController)
	{
		aiController->SetNewLocation(destination->GetActorLocation());
		aiController->SetToPlayer(true);
	}
}

void APrisonerQuestNPC::DestinationOverlap
(UPrimitiveComponent* overlappedComponent_,
	AActor* otherActor_,
	UPrimitiveComponent* otherComp_,
	int32 otherBodyIndex_,
	bool bFromSweep_,
	const FHitResult &sweepResult_)
{
	if (otherActor_ != nullptr && otherActor_ != this && otherComp_ != nullptr)
	{
		//Reached the destination, end the zombification
		AQuestNPC* destination = Cast<AQuestNPC>(otherActor_);
		if (destination)
		{
			if(!QuestManager::GetInstance()->IsQuestCompleted(questName))
				QuestManager::GetInstance()->CompleteQuest(questName, endQuestDescription);
			EndZombify();
		}
		else
		{
			//If the player shoots at you, die
			AAimProjectile* proj = Cast<AAimProjectile>(otherActor_);

			if (proj && !bDead)
			{
				bDead = true;
				proj->Destroy();
				if (autoDialogue)
					autoDialogue->Destroy();
				Death();
			}
		}
	}
}