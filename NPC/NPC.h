// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "NPC.generated.h"

USTRUCT()
struct FDialogue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Dialogue")
		TArray<FString> linesArray;
};

UCLASS()
class NECROMANCER_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Dialogue")
		TArray<FDialogue> lines; //First index is dialogueStageIndex and second index is the actual line/name
	UPROPERTY(EditAnywhere, Category = "Dialogue")
		TArray<FDialogue> names;
	UPROPERTY(EditAnywhere, Category = "Dialogue")
		UBoxComponent* dialogueBox;
	int lineIndex;
	int dialogueStageIndex; //Some NPCs, like Quest NPCs will only say certain lines when giving the quest for example

public:	
	virtual void GetNextLineAndName(FString& name_, FString& line_);
	virtual void ResetNPCDialogue();

};
