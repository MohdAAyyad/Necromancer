// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "BloodPool.generated.h"

UCLASS()
class NECROMANCER_API ABloodPool : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABloodPool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	USceneComponent* root;
	UPROPERTY(EditAnywhere, Category = BoxCollision)
	UBoxComponent* box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* bpMaster;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bAbsorbed; //Stop increasing the pool's size when absorbed
	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* particles;

	UPROPERTY(EditDefaultsOnly, Category = Wall)
		TSubclassOf<class ABloodWall> wall;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) { Super::Tick(DeltaTime); }
	virtual bool React() override { return true; };
	virtual void Interact() override;
	virtual void Interact(float& value_, bool bp_) override {};
	virtual void Absorbed();//Called when a BP is absorbed or used
	virtual void WallAction(FRotator rotater_) override;

	void DestroyBP();

};
