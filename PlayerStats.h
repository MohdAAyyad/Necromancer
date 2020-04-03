// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStats.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NECROMANCER_API UPlayerStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStats();

	float& GetMaxHP();
	float& GetHP();
	float GetHPPercent();

	void AddToMaxHP(float ); //Called from UI

	float& GetMaxBP();
	float& GetBP();
	float GetBPPercent();

	void AddToMaxBP(float); //Called from UI

	float GetBaseDamage();
	float GetBaseMagicDamage();

	void AddToBaseDamage(float amount_);
	void AddToBaseMagicDamage(float amount_);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float maxHP;
	float hp;
	float maxBP;
	float bp;
	float baseDamage;
	float baseMagicDamage;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
