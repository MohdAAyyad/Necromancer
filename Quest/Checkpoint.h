// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Checkpoint.generated.h"

UCLASS()
class NECROMANCER_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Root")
		USceneComponent* root;
	UPROPERTY(EditAnywhere,Category = "Sphere")
		USphereComponent* sphere;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystemComponent* inactiveParticles;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystemComponent* activeParticles;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float hpToRestore;
	UPROPERTY(EditAnywhere, Category = "Stats")
		float bpToRestore;

	UPROPERTY(EditAnywhere, Category = "AudioComponent")
		UAudioComponent* audioComponent;
	UPROPERTY(EditAnywhere, Category = "AudioComponent")
		USoundBase* sound;

	FTimerHandle timerHandleToDisableP1;
	float timeToDisableP1;

	void DisableInactiveParticles();

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* overlappedComponent_,
		AActor* otherActor_,
		UPrimitiveComponent* otherComp_,
		int32 otherBodyIndex_,
		bool bFromSweep_,
		const FHitResult &sweepResult_);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	inline FVector GetCheckpointLocation() { return GetActorLocation(); };


};
