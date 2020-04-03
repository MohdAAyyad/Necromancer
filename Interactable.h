// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NECROMANCER_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Interact() = 0; //Interact with the object
	virtual void Interact(float& value_, bool bp_) = 0;
	virtual bool React() = 0; //React for the crosshair
	virtual void WallAction(FRotator rotater_) = 0; //Bloodpools spawn walls, walls destroy themselves
};
