// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "./Spells/Blood/SummonBase.h"
#include "EnemyBase.h"

AEnemyAIController::AEnemyAIController()
{
	playerKey = "Target";
	newLocationKey = "MoveToThis";
	reloadKey = "Reloading";
	deathKey = "Dead";
	strafeKey = "Strafing";
	toPlayerKey = "ToPlayer";
	castKey = "Casting";
	hitKey = "Hit";
	specialKey = "Special";
	attackKey = "Attack";
	sittingKey = "Sitting";
	evadingKey = "Evading";
	blackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	behaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("EnemyBaseBehaviorTree"));
}

void AEnemyAIController::OnPossess(APawn* pawn_)
{
	Super::OnPossess(pawn_);

	AEnemyBase* enemy = Cast<AEnemyBase>(pawn_);

	if (enemy)
	{
		if (enemy->enemyBT)
		{
			if (enemy->enemyBT->BlackboardAsset)
			{
				blackboardComponent->InitializeBlackboard(*(enemy->enemyBT->BlackboardAsset));
				behaviorTreeComponent->StartTree(*(enemy->enemyBT));
			}
		}
	}
	else
	{
		ASummonBase* summon = Cast<ASummonBase>(pawn_);

		if (summon)
		{
			if (summon->summonBT)
			{
				if (summon->summonBT->BlackboardAsset)
				{
					blackboardComponent->InitializeBlackboard(*(summon->summonBT->BlackboardAsset));
					behaviorTreeComponent->StartTree(*(summon->summonBT));
				}
			}
		}
	}
}

void AEnemyAIController::SetSeenTarget(APawn* pawn_)
{
	if (blackboardComponent)
	{
		blackboardComponent->SetValueAsObject(playerKey, pawn_);
	}

	if (pawn_)
		SetFocus(pawn_);
	else
		ClearFocus(EAIFocusPriority::Gameplay);
}

void AEnemyAIController::SetDead()
{
	if (blackboardComponent)
	{
		blackboardComponent->SetValueAsBool(deathKey, true);
	}
}

void AEnemyAIController::ResetDead()
{
	if (blackboardComponent)
	{
		blackboardComponent->SetValueAsBool(deathKey, false);
		blackboardComponent->SetValueAsBool(hitKey, false);
		blackboardComponent->SetValueAsBool(specialKey, false);
		blackboardComponent->SetValueAsBool(castKey, false);
		blackboardComponent->SetValueAsBool(reloadKey, false);
		blackboardComponent->SetValueAsBool(strafeKey, false);
		blackboardComponent->SetValueAsBool(toPlayerKey, false);
	}
}

void AEnemyAIController::SetNewLocation(FVector loc_)
{
	if (blackboardComponent)
	{
		blackboardComponent->SetValueAsVector(newLocationKey, loc_);
		loc_ = blackboardComponent->GetValueAsVector(newLocationKey);

	}
}


FVector AEnemyAIController::GetPlayerLocation()
{
	if (blackboardComponent)
	{
		APawn* player_ = Cast<APawn>(blackboardComponent->GetValueAsObject(playerKey));

		if (player_)
			return player_->GetActorLocation();
	}

	return FVector::ZeroVector;
}

void AEnemyAIController::SetReload(bool value_)
{
	if (blackboardComponent)
	{
		blackboardComponent->SetValueAsBool(reloadKey, value_);
	}
}

void AEnemyAIController::SetStrafe(bool value_)
{
	if (blackboardComponent)
	{
		blackboardComponent->SetValueAsBool(strafeKey, value_);
	}
 }
 void AEnemyAIController::SetToPlayer(bool value_)
 {
	 if (blackboardComponent)
	 {
		 blackboardComponent->SetValueAsBool(toPlayerKey, value_);
	 }
 }


 UObject* AEnemyAIController::GetPlayer()
 {
	 if (blackboardComponent)
	 {
		 return blackboardComponent->GetValueAsObject(playerKey);
	 }

	 return nullptr;
 }

 void AEnemyAIController::SetCast(bool value_)
 {
	 if (blackboardComponent)
	 {
		 blackboardComponent->SetValueAsBool(castKey, value_);
	 }
 }

 void AEnemyAIController::SetHit(bool value_)
 {
	 if (blackboardComponent)
	 {
		 blackboardComponent->SetValueAsBool(hitKey, value_);
	 }
 }

 void AEnemyAIController::SetSpecial(bool value_)
 {
	 if (blackboardComponent)
	 {
		 blackboardComponent->SetValueAsBool(specialKey, value_);
	 }
 }

void AEnemyAIController::SetAttack(bool value_)
{
	 if (blackboardComponent)
	 {
		 blackboardComponent->SetValueAsBool(attackKey, value_);
	 }
}

void AEnemyAIController::SetSitting(bool value_)
{
	if (blackboardComponent)
	{
		blackboardComponent->SetValueAsBool(sittingKey, value_);
	}
}

void AEnemyAIController::SetEvading(bool value_)
{
	if (blackboardComponent)
	{
		blackboardComponent->SetValueAsBool(evadingKey, value_);
	}
}
