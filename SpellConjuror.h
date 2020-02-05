// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Spells.h"
#include "StatusEffects.h"
#include "SpellConjuror.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NECROMANCER_API USpellConjuror : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpellConjuror();
	void ConjourAimSpell(EAimSpells spell_, FVector spawnLocation_, bool increaseHP_, float& currentHP_, float damage, EStatusEffects status_, EStatusDuration duration_);
	void ConjourBloodSpell(EBloodSpells spell_, FVector spawnLocation_, bool increaseHP_, float& currentHP_, float damage, EStatusEffects status_, EStatusDuration duration_);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
