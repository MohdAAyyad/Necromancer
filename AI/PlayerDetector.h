// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.h"
#include "Components/BoxComponent.h"
#include "PlayerDetector.generated.h"

UCLASS()
class NECROMANCER_API APlayerDetector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerDetector();

protected:
	UPROPERTY(EditAnywhere, Category = "BoxCollision")
		UBoxComponent* box;

	UPROPERTY(EditAnywhere, Category = "Enemies")
		TArray<AEnemyBase*> enemies;

	void BeginPlay() override;

	UFUNCTION()
		void PlayerOverlap
		(UPrimitiveComponent* overlappedComponent_,
			AActor* otherActor_,
			UPrimitiveComponent* otherComp_,
			int32 otherBodyIndex_,
			bool bFromSweep_,
			const FHitResult &sweepResult_);

};
