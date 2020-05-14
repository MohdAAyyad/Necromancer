// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Spells.h"
#include "StatusEffects.h"
#include "AimProjectile.h"
#include "Spells/Blood/SummonBase.h"
#include "SpellConjuror.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NECROMANCER_API USpellConjuror : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpellConjuror();
	void ConjurAimSpell(EAimSpells spell_, FVector spawnLocation_, FRotator spawnRotation_, bool increaseHP_, float& currentHP_, float damage, EStatusEffects status_, EStatusDuration duration_, APlayerController* playerController_, TSubclassOf<UPlayerCameraShake> cameraShake_);
	void ConjurBloodSpell(EBloodSpells spell_, FVector spawnLocation_, FRotator spawnRotation_, bool increaseHP_, float& currentHP_, float damage, EStatusEffects status_, EStatusDuration duration_, APlayerController* playerController_, TSubclassOf<UPlayerCameraShake> cameraShake_);
	void ConjurInnateSpell(EInnateSpells spell_, FVector spawnLocation_, EStatusEffects status_, EStatusDuration duration_);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Spells)
		TArray<TSubclassOf<class AAimProjectile>> aimSpellProjectiles;

	UPROPERTY(EditDefaultsOnly, Category = Spells)
		TArray<TSubclassOf<class AAimProjectile>>  bloodSpellProjectiles;

	UPROPERTY(EditDefaultsOnly, Category = Spells)
		TArray<TSubclassOf<class ASummonBase>> summons;
		
};
