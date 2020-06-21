// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/UnrealString.h"
#include "Components/BoxComponent.h"
#include "DialogueHandle.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NECROMANCER_API UDialogueHandle : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueHandle();

protected:
	void BeginPlay() override;
	FString nextDialogue; // The dialogue chunk to be displayed next
	FString nextName; //The name of the person speaking
	TArray<TCHAR> nextDialogueChars;
	int nextDialogueCharIndex; //Keeps track of each individual character
	bool bEngagedInADialogue;
	class UPlayerUIController* uictrl;
	float timeToNextChar;
	FTimerHandle timeToNextCharHandle;

	bool bWithinRangeOfAnNPC; 
	bool displayingDialogue; //If we're displaying the dialogue one character a time and the player presses E, display the whole thing

	void UpdateDialogueUI();

	class ANPC* npc;
	class ANecromancerCharacter* necro;



public:	
	void SetNextDialogue(); //Called by the NPC to updated the dialogue
	bool IsInDialogue() { return bEngagedInADialogue; }; //If in dialogue, the player cannot move or open the pause menu
	void SetUICTRL(class UPlayerUIController* uictrl_);
	void SetOwnerPlayer(class ANecromancerCharacter* necro_);
	void ReadNextLine();
	void ProcessInput();



	UFUNCTION()
		void DialogueOverlap
		(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult &SweepResult);

	UFUNCTION()
		void EndDialogueOverlap
		(UPrimitiveComponent* overlappedComp_,
			AActor* otherActor_,
			UPrimitiveComponent* otherComp_,
			int32 otherBodyIndex_);

};
