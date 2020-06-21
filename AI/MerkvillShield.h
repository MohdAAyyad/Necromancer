// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "MerkvillShield.generated.h"

UCLASS()
class NECROMANCER_API AMerkvillShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMerkvillShield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Root)
		USceneComponent* root;
	UPROPERTY(EditAnywhere, Category = Box)
		UBoxComponent* box;
	UPROPERTY(EditAnywhere, Category = Particles)
		UParticleSystemComponent* shieldParticles0;
	UPROPERTY(EditAnywhere, Category = Particles)
		UParticleSystemComponent* shieldParticles1;
	UPROPERTY(EditAnywhere, Category = Particles)
		UParticleSystemComponent* shieldParticles2;
	UPROPERTY(EditAnywhere, Category = Particles)
		UParticleSystemComponent* shieldParticles3;
	UPROPERTY(EditAnywhere, Category = Particles)
		UParticleSystemComponent* shieldParticles4;


	UFUNCTION()
	void OverlapWithPlayerPorjectiles
	(UPrimitiveComponent* overlappedComponent_,
		AActor* otherActor_,
		UPrimitiveComponent* otherComp_,
		int32 otherBodyIndex_,
		bool bFromSweep_,
		const FHitResult &sweepResult_);
public:	
	void activateParticles();
	void DestroySelf();
};
