// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "StatusEffects.h"
#include "DestructibleProp.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCameraShake.h"
#include "TimerManager.h"
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

	virtual inline void SetDamage(float amount_) { damage = amount_; };
	inline float GetDamage() { return damage; };
	void GetSmallerOverTime(); //Called when the projectile is abosrobed by Crypto
	void UpdateRotation(FRotator rotation_);
	void UpdateVelocity(FVector vel_);

	void SetControllerAndCameraShake(APlayerController* playerController_, TSubclassOf<UPlayerCameraShake> cameraShake_);
protected:

	FTimerHandle timeHandleToGetReallySmall;
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
	UPROPERTY(EditAnywhere, Category = "Impact")
		UParticleSystem* impact;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInterface* decalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float damage;

	EStatusEffects effect;
	EStatusDuration duration;
	float durationInSeconds;

	//Filled by the player code when the proj is spawned
	APlayerController* playerController;
	TSubclassOf<UPlayerCameraShake> cameraShake;
	bool bBeingAbsorbed; //Turned to true when crypto absorbs a projectiles. Makes sure the projectile doesn't damage the enemy

	UFUNCTION()
		virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult &SweepResult);


	virtual void BindSphere();

	virtual void AddDamageModifier(float damageModifier_);
	virtual void SetEffectAndDuration(EStatusEffects effect_, EStatusDuration duration_);
};
