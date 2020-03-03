// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../SpellsInventory.h"
#include "../Spells.h"
#include "Engine/Canvas.h"
#include "PlayerUIController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NECROMANCER_API UPlayerUIController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerUIController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:

	float hpPercent;
	float bloodPercent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bGameIsPaused;

	UPROPERTY(EditAnywhere)
		TArray<UTexture2D*> spellTextures;

	UFUNCTION(BlueprintCallable)
		float UpdateHealth() const; //Returns health percentage to update UI
	UFUNCTION(BlueprintCallable)
		float UpdateBlood() const; //Returns blood percentage to update UI

	UTexture2D* LookForAimTexture(EAimSpells spell_); //Looks through a switch statement to get the correct texture back
	UTexture2D* LookForBloodTexture(EBloodSpells spell_); //Looks through a switch statement to get the correct texture back
	UTexture2D* LookForInnateTexture(EInnateSpells spell_);
public:
	UFUNCTION(BlueprintCallable)
		void EquipAimSpell(const int index_);
	UFUNCTION(BlueprintCallable)
		void EquipBloodSpell(const int index_);
	UFUNCTION(BlueprintCallable)
		void EquipInnateSpell(const int index_);
	UFUNCTION(BlueprintCallable)
		void PauseGame();
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetAimTexture(int index_); //Called from the UMG to get the texture
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetEquippedAimTexture(int index_); //Called from the UMG to get the texture
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetBloodTexture(int index_); //Called from the UMG to get the texture
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetEquippedBloodTexture(int index_); //Called from the UMG to get the texture
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetInnateTexture(int index_); //Called from the UMG to get the texture
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetEquippedInnateTexture(); //Called from the UMG to get the texture

	UFUNCTION(BlueprintCallable)
		void RemoveAimSpell(int index_);

	UFUNCTION(BlueprintCallable)
		void RemoveBloodSpell(int index_);

	UFUNCTION(BlueprintCallable)
		void RemoveInnateSpell();

	void SetHealthPercentage(float percent_);
	void SetBloodPercentage(float percent_);
		
};
