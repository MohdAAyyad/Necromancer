// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Crypto.h"
#include "CryptoAbsorbtionSphere.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NECROMANCER_API UCryptoAbsorbtionSphere : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCryptoAbsorbtionSphere();
	void SetCryptoRef(ACrypto* ref_);
	void ActivateSpheres();
	void DeactivateSpheres();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	ACrypto* cryptoRef;
	UPROPERTY(EditAnywhere, Category = "Abosorption")
		USphereComponent* outerSphere;
	UPROPERTY(EditAnywhere, Category = "Abosorption")
		USphereComponent* innerSphere;

	UFUNCTION()
	void OuterSphereOnOverlap(UPrimitiveComponent* overlappedComponent_,
		AActor* otherActor_,
		UPrimitiveComponent* otherComp_,
		int32 otherBodyIndex_,
		bool bFromSweep_,
		const FHitResult &sweepResult_);

	UFUNCTION()
	void InnerSphereOnOverlap(UPrimitiveComponent* overlappedComponent_,
		AActor* otherActor_,
		UPrimitiveComponent* otherComp_,
		int32 otherBodyIndex_,
		bool bFromSweep_,
		const FHitResult &sweepResult_);
};
