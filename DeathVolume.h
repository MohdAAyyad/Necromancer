// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "DeathVolume.generated.h"

UCLASS()
class NECROMANCER_API ADeathVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeathVolume();
private:
	UPROPERTY(EditAnywhere)
	USceneComponent* root;
	UPROPERTY(EditAnywhere)
	UBoxComponent* box;

	UFUNCTION()
		void OnPlayerOverlap(UPrimitiveComponent* overlappedComponent_,
			AActor* otherActor_,
			UPrimitiveComponent* otherComp_,
			int32 otherBodyIndex_,
			bool bFromSweep_,
			const FHitResult &sweepResult_);
protected:
	void BeginPlay() override;


};
