// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class NECROMANCER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	class UBehaviorTreeComponent* behaviorTreeComponent;
	class UBlackboardComponent* blackboardComponent;

public:
	AEnemyAIController();
	UPROPERTY(VisibleAnywhere, Category = Blackboard)
		FName playerKey; //Player
	UPROPERTY(VisibleAnywhere, Category = Blackboard)
		FName newLocationKey; //Move to this location
	UPROPERTY(VisibleAnywhere, Category = Blackboard)
		FName reloadKey; //Move to this location
	UPROPERTY(VisibleAnywhere, Category = Blackboard)
		FName deathKey; //Move to this location
	UPROPERTY(VisibleAnywhere, Category = Blackboard)
		FName strafeKey; //Move to this location
	UPROPERTY(VisibleAnywhere, Category = Blackboard)
		FName toPlayerKey; //Move to this location
	UPROPERTY(VisibleAnywhere, Category = Blackboard)
		FName castKey; //Move to this location
	UPROPERTY(VisibleAnywhere, Category = Blackboard)
		FName hitKey; //Move to this location
	UPROPERTY(VisibleAnywhere, Category = Blackboard)
		FName specialKey; //Move to this location
	UPROPERTY(VisibleAnywhere, Category = Blackboard)
		FName attackKey; //Move to this location

	virtual void OnPossess(APawn* pawn_) override;
	virtual void SetSeenTarget(APawn* pawn_);
	virtual void SetDead();
	virtual void ResetDead();
	virtual void SetNewLocation(FVector loc_);
	virtual void SetReload(bool value_);
	virtual void SetStrafe(bool value_);
	virtual void SetToPlayer(bool value_);
	virtual void SetCast(bool value_);
	virtual void SetHit(bool value_);
	virtual void SetSpecial(bool value_);
	virtual void SetAttack(bool value_);
	virtual FVector GetPlayerLocation();
	virtual UObject* GetPlayer();
};
