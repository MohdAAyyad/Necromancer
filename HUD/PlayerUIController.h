// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../SpellsInventory.h"
#include "../Spells.h"
#include "PlayerUIController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NECROMANCER_API UPlayerUIController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerUIController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:

	float hpPercent;
	float bloodPercent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bGameIsPaused;

	UFUNCTION(BlueprintCallable)
		float UpdateHealth() const; //Returns health percentage to update UI
	UFUNCTION(BlueprintCallable)
		float UpdateBlood() const; //Returns blood percentage to update UI
public:
	UFUNCTION(BlueprintCallable)
		void EquipAimSpell(EAimSpells spell_);
	UFUNCTION(BlueprintCallable)
		void PauseGame();

	void SetHealthPercentage(float percent_);
	void SetBloodPercentage(float percent_);
		
};
