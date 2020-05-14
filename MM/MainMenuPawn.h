// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MMPawnAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "MainMenuPawn.generated.h"

UCLASS()
class NECROMANCER_API AMainMenuPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainMenuPawn();

	UFUNCTION(BlueprintCallable)
		void Rise(bool value_); //Called when the player presses play game 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UMMPawnAnimInstance* animInstance;
	
	UPROPERTY(EditAnywhere, Category = "Mesh")
		USkeletalMeshComponent* mesh;

	UPROPERTY(EditAnywhere, Category = "RootComponent")
		USceneComponent* root;

	UFUNCTION(BlueprintCallable)
		void MakeEnemiesRise(); //Called when the player presses play game 
	

};
