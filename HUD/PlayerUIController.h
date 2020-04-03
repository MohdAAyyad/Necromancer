// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../SpellsInventory.h"
#include "../Spells.h"
#include "Engine/Canvas.h"
#include "../EXPManager.h"
#include "../PlayerStats.h"
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
	UPlayerStats* stats;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bGameIsPaused;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bSkillTreeMenuIsShown;

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
	void SetPlayerStatsPtr(UPlayerStats* stats_) { stats = stats_; };

	UFUNCTION(BlueprintCallable)
		void EquipAimSpell(const int index_);
	UFUNCTION(BlueprintCallable)
		void EquipBloodSpell(const int index_);
	UFUNCTION(BlueprintCallable)
		void EquipInnateSpell(const int index_);
	UFUNCTION(BlueprintCallable)
		void PauseGame();
	UFUNCTION(BlueprintCallable)
		void SkillTreeMenu();
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

	//Unlock Spells
	UFUNCTION(BlueprintCallable)
	void UnlockAimSpell(int index_);
	UFUNCTION(BlueprintCallable)
	void UnlockBloodSpell(int index_);
	UFUNCTION(BlueprintCallable)
	void UnlockInnateSpell(int index_);
		

	//EXP
	UFUNCTION(BlueprintCallable)
		int GetCurrentSkillPoints();
	UFUNCTION(BlueprintCallable)
		int GetCurrentEXP();
	UFUNCTION(BlueprintCallable)
		int GetEXPToLevelUp();

	//Name And Description

	UFUNCTION(BlueprintCallable)
		FString GetAimName(int index_); //Returns unlocked spell name
	UFUNCTION(BlueprintCallable)
		FString GetAimDescription(int index_);//Returns unlocked spell Description
	UFUNCTION(BlueprintCallable)
		FString GetLockedAimName(int index_);//Returns locked spell name
	UFUNCTION(BlueprintCallable)
		FString GetLockedAimDescription(int index_);//Returns locked spell Description

	UFUNCTION(BlueprintCallable)
		FString GetBloodName(int index_);
	UFUNCTION(BlueprintCallable)
		FString GetBloodDescription(int index_);
	UFUNCTION(BlueprintCallable)
		FString GetLockedBloodName(int index_);
	UFUNCTION(BlueprintCallable)
		FString GetLockedBloodDescription(int index_);

	UFUNCTION(BlueprintCallable)
		FString GetInnateName(int index_);
	UFUNCTION(BlueprintCallable)
		FString GetInnateDescription(int index_);
	UFUNCTION(BlueprintCallable)
		FString GetLockedInnateName(int index_);
	UFUNCTION(BlueprintCallable)
		FString GetLockedInnateDescription(int index_);


	//Stats
	UFUNCTION(BlueprintCallable)
		int GetBaseDamage();
	UFUNCTION(BlueprintCallable)
		int GetBaseMagicDamage();
	UFUNCTION(BlueprintCallable)
		int GetMaxHP();
	UFUNCTION(BlueprintCallable)
		int GetMaxBP();

	UFUNCTION(BlueprintCallable)
		void AddBaseDamage();
	UFUNCTION(BlueprintCallable)
		void AddMagicDamage();
	UFUNCTION(BlueprintCallable)
		void AddHP();
	UFUNCTION(BlueprintCallable)
		void AddBP();
};
