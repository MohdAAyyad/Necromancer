// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "DestructibleProp.h"
#include "BloodWall.generated.h"

UCLASS()
class NECROMANCER_API ABloodWall : public ADestructibleProp, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABloodWall();

protected:

	void BeginPlay() override;
	void Tick(float deltaTime_) override;
	FVector targetLocation;
	FVector currentLocation;

public:
	void TakeDamage(float damage_) override;

	void Interact() override {} ; //Interact with the object
	void Interact(float& value_, bool bp_) override {} ;
	bool React() override { return true; } ; //React for the crosshair
	void WallAction(FRotator rotater_) override; //Bloodpools spawn walls, walls destroy themselves



};
