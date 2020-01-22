// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "AimProjectile.generated.h"

UCLASS()
class NECROMANCER_API AAimProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAimProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Custom

protected:

	UPROPERTY(EditAnywhere, Category = Sphere)
		USphereComponent* sphere;
	UPROPERTY(EditAnywhere, Category = mesh)
		UStaticMeshComponent* mesh;
	UPROPERTY(EditAnywhere)
		USceneComponent* root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* movement;
	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* particles;


};
