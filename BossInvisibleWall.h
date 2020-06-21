// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "BossInvisibleWall.generated.h"

UCLASS()
class NECROMANCER_API ABossInvisibleWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossInvisibleWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Root)
		USceneComponent* root;
	UPROPERTY(EditAnywhere, Category = Box)
		UBoxComponent* box;

	UPROPERTY(EditAnywhere, Category = Box)
		UParticleSystemComponent* particles;

	UFUNCTION()
		void EndOverlap
		(UPrimitiveComponent* overlappedComp_,
			AActor* otherActor_,
			UPrimitiveComponent* otherComp_,
			int32 otherBodyIndex_);

};
