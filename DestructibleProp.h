// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "DestructibleComponent.h"
#include "TimerManager.h"
#include "Sound/SoundBase.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "DestructibleProp.generated.h"

UCLASS()
class NECROMANCER_API ADestructibleProp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructibleProp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		USceneComponent* root;

	UPROPERTY(EditAnywhere, Category = Mesh)
		class UDestructibleComponent* mesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		UBoxComponent* box;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float hp;

	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundBase* destructionSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
		float volume;
	UPROPERTY(EditAnywhere, Category = "Sound")
		float pitch;

	FTimerHandle timeToDestroyHandle;
	void DelayedDestroy();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PropTakeDamage(float damage_);

};
